set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
#find_package(Gpgme REQUIRED)

if(NOT GPGME_FOUND)
  include(ExternalProject)
  ExternalProject_Add(Gpgme
    SOURCE_DIR  ${PROJECT_SOURCE_DIR}/dep/gpgme/gpgme
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --disable-gpg-test
    BUILD_COMMAND ${MAKE_EXE}
  )

  # need to test that this now finds it when built
  find_package(Gpgme REQUIRED)
endif()
