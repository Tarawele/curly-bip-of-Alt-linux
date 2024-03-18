#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>
#include <unordered_map>

// Struct to store API response
struct ResponseData {
    std::string data;
};

// Package struct 
struct Package {
  std::string name;
  int epoch;
  std::string version;
  std::string release;
  std::string arch;
  // other fields...
};

// Parse JSON and extract packages
std::vector<Package> parsePackages(const std::string& json) {

  // Parse JSON
  rapidjson::Document doc;
  doc.Parse(json.c_str());

  // Get packages array
  rapidjson::GenericArray<false, rapidjson::GenericValue<rapidjson::UTF8<> > > packages = doc["packages"].GetArray(); 

  // Extract packages
  std::vector<Package> pkgs;
  for(auto& pkg : packages) {
    Package p;
    p.name = pkg["name"].GetString();
    p.epoch = pkg["epoch"].GetInt(); 
    p.version = pkg["version"].GetString();
    p.release = pkg["release"].GetString();
    p.arch = pkg["arch"].GetString();

    // Extract other fields

    pkgs.push_back(p);
  }

  return pkgs;
}
// Callback function to write response data
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  ResponseData *mem = (ResponseData *)userp;
  mem->data.append((char *)contents, realsize);
  return realsize;
}

// Function to send an HTTP GET request and return the response data
std::string getPackages(const std::string &branch) {
    std::string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch;

    CURL *curl = curl_easy_init();
    if (curl) {
        ResponseData response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error during curl_easy_perform(): " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return "";
        }

        curl_easy_cleanup(curl);
        return response.data;
    } else {
        std::cerr << "Error initializing cURL" << std::endl;
        return "";
    }
}

void comparePackages(const std::vector<Package>& packagesBranch1, const std::vector<Package>& packagesBranch2) {
    // Create unordered maps to store packages for quick lookups
    std::unordered_map<std::string, Package> pkgMapBranch1;
    std::unordered_map<std::string, Package> pkgMapBranch2;

    // Populate the unordered map for branch 1 with package name + architecture as key
    for (const auto& pkg : packagesBranch1) {
        pkgMapBranch1[pkg.name + "_" + pkg.arch] = pkg;
    }

    // Populate the unordered map for branch 2 with package name + architecture as key
    for (const auto& pkg : packagesBranch2) {
        pkgMapBranch2[pkg.name + "_" + pkg.arch] = pkg;
    }

    // Compare packages in branch 1 with branch 2
    for (const auto& entry : pkgMapBranch1) {
        const std::string key = entry.first;
        const Package& pkg1 = entry.second;

        // Check if the package is missing in branch 2
        if (pkgMapBranch2.find(key) == pkgMapBranch2.end()) {
            std::cout << "Package " << pkg1.name << " is missing in Branch 2 for architecture " << pkg1.arch << "\n";
        }
        else {
            const Package& pkg2 = pkgMapBranch2[key];

            // Compare versions and epochs to determine higher version
            if (pkg1.epoch > pkg2.epoch ||
                (pkg1.epoch == pkg2.epoch && pkg1.version > pkg2.version) ||
                (pkg1.epoch == pkg2.epoch && pkg1.version == pkg2.version && pkg1.release > pkg2.release)) {
                std::cout << "Package " << pkg1.name << " has a higher version in Branch 1 for architecture " << pkg1.arch << "\n";
            } else {
              std::cout << "Package " << pkg2.name << " has a higher version in Branch 2 for architecture " << pkg2.arch << "\n";
            }
        }
    }

    // Identify packages missing in branch 1
    for (const auto& entry : pkgMapBranch2) {
        const std::string key = entry.first;
        const Package& pkg2 = entry.second;

        // Check if the package is missing in branch 1
        if (pkgMapBranch1.find(key) == pkgMapBranch1.end()) {
            std::cout << "Package " << pkg2.name << " is missing in Branch 1 for architecture " << pkg2.arch << "\n";
        }
    }
}
int main() {
  std::string branch1 = "p10";
  std::string branch2 = "p9";

  // Get response data from an HTTP GET request
  const std::string responseData1 = getPackages(branch1);
  const std::string responseData2 = getPackages(branch2);

  // Parse JSON and extract packages
  std::vector<Package> packagesBranch1 = parsePackages(responseData1);
  std::vector<Package> packagesBranch2 = parsePackages(responseData2);

  comparePackages(packagesBranch1, packagesBranch2);

  return 0;
}