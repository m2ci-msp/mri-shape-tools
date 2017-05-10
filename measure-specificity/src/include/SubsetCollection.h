#ifndef __SUBSET_COLLECTION_H__
#define __SUBSET_COLLECTION_H__

#include <string>
#include <vector>
#include <fstream>

#include <json/json.h>

#include "mesh/Mesh.h"
#include "Subset.h"

class SubsetCollection{

public:

  /*-----------------------------------------------------------------------*/

  SubsetCollection() {
  }

  /*-----------------------------------------------------------------------*/

  void add_subset(const std::string& name, std::vector<int>& indices) {
    this->subsets.push_back(Subset(name, indices));
  }

  /*-----------------------------------------------------------------------*/

  void add_subset(const std::string& fileName) {

    // try to open file
    std::ifstream inFile(fileName);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open subset file.");
    }

    // read json file
    Json::Value subset;
    Json::Reader reader;
    reader.parse(inFile, subset);

    std::string name = subset["name"].asString();

    std::vector<int> indices;

    for(const Json::Value& index: subset["indices"]) {
      indices.push_back(index.asInt());
    }

    this->subsets.push_back(Subset(name, indices));

    inFile.close();

  }

  /*-----------------------------------------------------------------------*/

  std::vector<Subset> get_subsets() const {
    return this->subsets;
  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  std::vector<Subset> subsets;

  /*-----------------------------------------------------------------------*/

};

#endif
