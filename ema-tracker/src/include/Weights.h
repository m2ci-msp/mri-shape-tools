#ifndef __WEIGHTS_H__
#define __WEIGHTS_H__

#include <string>
#include <fstream>

#include <armadillo>

class Weights{

public:

  /*-----------------------------------------------------------------------*/

  static arma::vec read(const std::string& fileName) {

    // try to open file
    std::ifstream inFile(fileName.c_str());

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open weights file.");
    }

    // read json file
    Json::Value root;
    Json::Reader reader;
    reader.parse(inFile, root);

    arma::vec result = arma::zeros(root.size());

    for( Json::ArrayIndex i = 0; i < root.size(); ++i) {

      result(i) = root[i].asDouble();

    }

    return result;

  }

  /*-----------------------------------------------------------------------*/

};

#endif
