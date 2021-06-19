//
//          Copyright (c) 2021, Anthony Bilinski
//
// This software is licensed under the MIT License. The LICENSE.md file
// describes the conditions under which this software may be distributed.
//
// Author: Anthony Bilinski
//


#ifndef GPGMEWRAPPER_H
#define GPGMEWRAPPER_H

#include "git2/buffer.h"
#include "gpgme.h"

#include <QString>
#include <string>
#include <memory>

struct git_repository;
namespace git
{
  class Config;
}

class GpgmeWrapper
{
public:
  static std::unique_ptr<GpgmeWrapper> makeGpgmeWrapper(); // returns null if gpgme_new errors
  ~GpgmeWrapper();
  GpgmeWrapper(GpgmeWrapper const&) = delete;
  std::unique_ptr<char> gpgSignCommit(git::Config confsnap, git_buf content) const;

private:
  GpgmeWrapper(std::unique_ptr<gpgme_ctx_t> ctx)
    : ctx{std::move(ctx)}
  {};
  QString getSigningKey(git::Config confsnap) const;
  std::unique_ptr<gpgme_ctx_t> ctx;
};

#endif // GPGMEWRAPPER_H
