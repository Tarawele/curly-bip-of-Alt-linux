#include <iostream>
#include <string>
#include <curl/curl.h>

#include "structDataPackages.hpp"

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