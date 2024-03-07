#include <curl/curl.h>
#include <iostream>
#include <string>

// Struct to store the response data
struct ResponseData {
  std::string data;
};

// Callback function to write received data to a string
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  ResponseData *mem = (ResponseData *)userp;
  mem->data.append((char *)contents, realsize);
  return realsize;
}

// Function to send an HTTP GET request and return the response body
std::string sendGetRequest(const std::string &url) {
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
    }

    curl_easy_cleanup(curl);
    return response.data;
  } else {
    std::cerr << "Error initializing cURL" << std::endl;
    return "";
  }
}

int main() {
  // Send a GET request to the specified URL
  std::string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus";
  std::string response = sendGetRequest(url);

  // Display the response body
  std::cout << "Response: " << response << std::endl;

  return 0;
}