#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  boost::asio::io_context io_context;

  po::options_description desc("allowed options");
  desc.add_options()("help", "produce help message");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  std::cout << "Hello World!" << std::endl;
  return 0;
}
