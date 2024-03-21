#ifndef BIPAC_H
#define BIPAC_H

// Parse JSON and extract packages
std::vector<Package> parsePackages(const std::string& json);

// Callback function to write response data
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

// Function to send an HTTP GET request and return the response data
std::string getPackages(const std::string &branch);

// Function to compare packages
void comparePackages(const std::vector<Package>& packagesBranch1, const std::vector<Package>& packagesBranch2, std::string br1, std::string br2);

#endif 