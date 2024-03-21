#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "structDataPackages.hpp"

void comparePackages(const std::vector<Package>& packagesBranch1, const std::vector<Package>& packagesBranch2, std::string branch1, std::string branch2) {
    // Create unordered maps to store packages for quick lookups
    std::unordered_map<std::string, Package> pkgMapBranch1;
    std::unordered_map<std::string, Package> pkgMapBranch2;

    // Populate the unordered map for branch 1 with package name + arch as key
    for (const auto& pkg : packagesBranch1) {
        pkgMapBranch1[pkg.name + "_" + pkg.arch] = pkg;
    }
    // Populate the unordered map for branch 2 with package name + arch as key
    for (const auto& pkg : packagesBranch2) {
        pkgMapBranch2[pkg.name + "_" + pkg.arch] = pkg;
    }
    // Compare packages in branch 1 with branch 2
    for (const auto& entry : pkgMapBranch1) {
        const std::string key = entry.first;
        const Package& pkg1 = entry.second;

        if (pkgMapBranch2.find(key) == pkgMapBranch2.end()) {
            std::cout << "\033[31mname:\033[0m " << pkg1.name << " is missing in Branch: " << branch2 << " \033[31mfor arch:\033[0m " << pkg1.arch << "\n";
        }
        else {
            const Package& pkg2 = pkgMapBranch2[key];
            // Compare versions to determine higher version
            if (pkg1.epoch > pkg2.epoch ||
                (pkg1.epoch == pkg2.epoch && pkg1.version > pkg2.version) ||
                (pkg1.epoch == pkg2.epoch && pkg1.version == pkg2.version && pkg1.release > pkg2.release)) {
                std::cout << "\033[32mname:\033[0m " << pkg1.name << " has a higher version-release in Branch: " << branch1 << " \033[32mfor arch:\033[0m " << pkg1.arch << "\n";
            } else {
              std::cout << "\033[32mname:\033[0m " << pkg2.name << " has a higher version-release in Branch: "<< branch2 << " \033[32mfor arch:\033[0m " << pkg2.arch << "\n";
            }
        }
    }
    // Identify packages missing in branch 1
    for (const auto& entry : pkgMapBranch2) {
      const std::string key = entry.first;
      const Package& pkg2 = entry.second;
      if (pkgMapBranch1.find(key) == pkgMapBranch1.end()) {
        std::cout << "\033[31mname:\033[0m " << pkg2.name << " is missing in Branch: " << branch1 << " \033[31mfor arch:\033[0m " << pkg2.arch << "\n";
      }
    }
}