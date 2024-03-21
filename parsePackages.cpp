#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>

#include "structDataPackages.hpp"


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
    pkgs.push_back(p);
  }

  return pkgs;
}