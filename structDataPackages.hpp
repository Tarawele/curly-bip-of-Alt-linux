#ifndef STRUCTDATA_H
#define STRUCTDATA_H

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

#endif