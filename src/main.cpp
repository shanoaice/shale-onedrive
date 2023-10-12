#include <iostream>
#include <string>

#include "boost/asio.hpp"
#include "mimalloc-new-delete.h"
#include "mimalloc.h"

#include "onedrive.h"

#define FUSE_USE_VERSION 35

int main(int argc, char *argv[]) {
  boost::asio::io_context io_context;

  std::cout << "Hello World!" << std::endl;
  return 0;
}
