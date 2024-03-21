#include <iostream>
#include <string>
#include <vector>

#include "structDataPackages.hpp"
#include "bipac.hpp" // Shared library

int main(int argc, char **argv) {

  std::string branch1 = argv[1];
  std::string branch2 = argv[2];

  if (argc == 3) {
    // Get response data from an HTTP GET request
    const std::string responseData1 = getPackages(branch1);
    const std::string responseData2 = getPackages(branch2);

    // Parse JSON and extract packages
    std::vector<Package> packagesBranch1 = parsePackages(responseData1);
    std::vector<Package> packagesBranch2 = parsePackages(responseData2);

    comparePackages(packagesBranch1, packagesBranch2, branch1, branch2);
  } else {
    std::cout << "Please enter correctly two binaries packages' name" << std::endl;
  }

  return 0;
}