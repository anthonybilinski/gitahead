set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
#find_package(Gpgme QUIET)

if(NOT GPGME_FOUND)
  include(ExternalProject)

  message("CMAKE_CURRENT_SOURCE_DIR is ${CMAKE_CURRENT_SOURCE_DIR}")
  ExternalProject_Add(Gpgme
    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/gpgme
    CONFIGURE_COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/gpgme && ./autogen.sh COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gpgme/configure --disable-gpg-test
    BUILD_COMMAND ${MAKE_EXE}
    INSTALL_COMMAND ""
  )

  find_library(
      Gpgme
      NAMES libgpgme.so
      PATHS ${CMAKE_CURRENT_SOURCE_DIR}/gpgme/src/.libs
      REQUIRED
      NO_DEFAULT_PATH #debug, not needed since this should be skipped otherwise
  )

endif()
