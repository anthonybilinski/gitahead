//
//          Copyright (c) 2021, Anthony Bilinski
//
// This software is licensed under the MIT License. The LICENSE.md file
// describes the conditions under which this software may be distributed.
//
// Author: Anthony Bilinski
//

#include "GpgmeWrapper.h"

#include "Config.h"
#include "Repository.h"

#include "git2/types.h"
#include "git2/config.h"
#include "gpgme.h"

#include <QString>
#include <string>
#include <memory>

std::unique_ptr<GpgmeWrapper> GpgmeWrapper::makeGpgmeWrapper()
{
  gpgme_check_version(nullptr); // must be called to init the library

  std::unique_ptr<gpgme_ctx_t> ctx(new gpgme_ctx_t);
  auto err = gpgme_new(ctx.get());
  if (err)
    return {};
  return std::unique_ptr<GpgmeWrapper>(new GpgmeWrapper{std::move(ctx)});
}

GpgmeWrapper::~GpgmeWrapper()
{
  gpgme_release(*ctx);
}

QString GpgmeWrapper::getSigningKey(git::Config confsnap) const
{
  if (!confsnap.value<bool>("commit.gpgsign"))
    return {};
  return confsnap.value<QString>("user.signingkey");
}

std::unique_ptr<char> GpgmeWrapper::gpgSignCommit(git::Config confsnap, git_buf content) const
{
  const auto sigkey = getSigningKey(confsnap);
  gpgme_error_t err;
  err = gpgme_new(ctx.get());
  if (err) {
    return {};
  }

  gpgme_set_armor(*ctx, true);

  gpgme_key_t key;
    err = gpgme_get_key(*ctx, sigkey.toStdString().c_str(), &key, true);
  if (err) {
    return {};
  }

  // gpgme_signers_clear(*ctx);
  err = gpgme_signers_add(*ctx, key);
  if (err) {
    return {};
  }

  gpgme_data_t plain;
  err = gpgme_data_new_from_mem(&plain, content.ptr, content.size, false);
  if (err) {
    return {};
  }
  gpgme_data_t gpgsig;
  err = gpgme_data_new(&gpgsig);
  if (err) {
    return {};
  }
  err = gpgme_op_sign(*ctx, plain, gpgsig, gpgme_sig_mode_t::GPGME_SIG_MODE_DETACH);
  if (err) {
    return {};
  }

  gpgme_data_seek (gpgsig, 0, SEEK_SET);
  gpgme_sign_result_t result = gpgme_op_sign_result(*ctx);
  char* sigstr = gpgme_data_release_and_get_mem(gpgsig, nullptr);
  return std::unique_ptr<char>(sigstr);
}
