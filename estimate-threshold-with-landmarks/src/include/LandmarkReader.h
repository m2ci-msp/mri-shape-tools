#ifndef __LANDMARK_READER_H__
#define __LANDMARK_READER_H__

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include <armadillo>
#include <json/json.h>

#include "image/Image.h"

class LandmarkReader{

public:

  static std::vector<arma::vec> read(const std::string& fileName, const Image& image) {

    // try to open file
    std::ifstream inFile(fileName);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open file.");
    }

    const double orgX = image.info().get_origin_x() / image.info().get_hx();
    const double orgY = image.info().get_origin_y() / image.info().get_hy();
    const double orgZ = image.info().get_origin_z() / image.info().get_hz();

    Json::Value root;

    inFile >> root;

    inFile.close();

    std::vector<arma::vec> result;

    for(const auto& value: root) {

      // map to local coordinate system
      const double x = value["x"].asDouble() - orgX;
      const double y = value["y"].asDouble() - orgY;
      const double z = value["z"].asDouble() - orgZ;

      result.push_back({x, y, z});

    }

    return result;

  }

};

#endif
