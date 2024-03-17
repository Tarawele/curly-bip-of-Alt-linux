#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>

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

int main() {
    // Example branch for getting packages
    std::string branch = "p10";

    // Get response data from an HTTP GET request
    const std::string responseData = getPackages(branch);

    // Parse JSON and extract packages
    std::vector<Package> packages = parsePackages(responseData);

    // Print packages
    for(auto& pkg: packages) {
        std::cout << "name:" << pkg.name << " " << "version: "<< pkg.version << std::endl; 
    }

    return 0;
}