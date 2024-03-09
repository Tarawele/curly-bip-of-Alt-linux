#include <curl/curl.h>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <vector>

// Struct to store the response data
struct ResponseData {
  std::string data;
};

// Define the Package structure representing a single package with name,
// version, and arch
struct Package {
  std::string name;
  std::string version;
  std::string arch;
};

// Callback function to write response data
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  ResponseData *mem = (ResponseData *)userp;
  mem->data.append((char *)contents, realsize);
  return realsize;
}

// Function to send an HTTP GET request and return the response data
std::string getPackages(const std::string &branch) {

  std::string url =
      "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch;

  CURL *curl = curl_easy_init();
  if (curl) {
    ResponseData response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "Error during curl_easy_perform(): "
                << curl_easy_strerror(res) << std::endl;
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

std::vector<Package> parsePackages(const std::string &json) {

  // Parse JSON into a document
  rapidjson::Document doc;
  doc.Parse(json.c_str());

  // Extract packages
  std::vector<Package> packages;
  for (auto &pkg : doc["packages"].GetArray()) {

    Package p;
    p.name = pkg["name"].GetString();
    p.version = pkg["version"].GetString();
    p.arch = pkg["arch"].GetString();

    packages.push_back(p);
  }

  return packages;
}

void displayPackages(const std::vector<Package> &packages) {

  for (const auto &package : packages) {
    std::cout << "name: " << package.name << std::endl;
    std::cout << "version: " << package.version << std::endl;
    std::cout << "arch: " << package.arch << std::endl;
    std::cout << std::endl;
  }
}

int main() {
  std::string namesPackages[2];
  std::cin >> namesPackages[0] >> namesPackages[1];

  std::string dataPackage1 = getPackages(namesPackages[0]);
  std::string dataPackage2 = getPackages(namesPackages[1]);

  std::vector<Package> package1 = parsePackages(dataPackage1);
  std::vector<Package> package2 = parsePackages(dataPackage2);

  displayPackages(package1);
  return 0;
}