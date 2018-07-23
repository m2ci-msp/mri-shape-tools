#ifndef __VERTEX_SUBSETS_H__
#define __VERTEX_SUBSETS_H__

#include <vector>
#include <stdexcept>

#include <json/json.h>

class VertexSubsets{

public:

  static std::vector< std::vector<unsigned int> > read_from(const std::vector<std::string>& fileNames) {
  
      // we use a set for storing the indices -> different regions might contain
      // the same index multiple times
      std::vector< std::vector<unsigned int> > subsets;
  
      for(const std::string fileName: fileNames) {
  
        std::vector<unsigned int> subset;
  
        // try to open file
        std::ifstream inFile(fileName);
  
        if(inFile.is_open() == false) {
  
          throw std::runtime_error("Could not open subset file " + fileName + ".");
  
        }
  
        // parse JSON file
        Json::Value vertexList;
        inFile >> vertexList;
        inFile.close();
  
        // iterate through list
        for(const Json::Value& value: vertexList) {
  
          subset.push_back( value.asInt() );
  
        } // end for vertexList
  
        subsets.push_back(subset);
  
      } // end for fileName
  
      return subsets;
  
    }

private:

  // make constructor private -> this is a class
  // only offering a static method
  VertexSubsets();

};

#endif
