#ifndef __LANDMARK_READER_H__
#define __LANDMARK_READER_H__

#include <vector>
#include <fstream>
#include <stdexcept>

#include <json/json.h>
#include <armadillo>

#include "image/Image.h"

class LandmarkReader{

private:

  /*-------------------------------------------------------------------------*/

  const Image& image;
  std::vector<arma::vec> points;

  /*-------------------------------------------------------------------------*/

public:

  /*-------------------------------------------------------------------------*/

  LandmarkReader(
                 const std::string& fileName, const Image& image) : image(image) {

    read_landmarks(fileName);

  }

  /*-------------------------------------------------------------------------*/

  const std::vector<arma::vec>& get() const {

    return this->points;

  }

private:

  /*-------------------------------------------------------------------------*/

  void read_landmarks(const std::string& fileName) {

    std::ifstream inFile(fileName);

    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open file.");
    }

    Json::Value root;
    Json::Reader reader;

    reader.parse(inFile, root);

    inFile.close();

    for(const auto& value: root) {

      const double x = value["x"].asDouble() * this->image.info().get_hx();
      const double y = value["y"].asDouble() * this->image.info().get_hy();
      const double z = value["z"].asDouble() * this->image.info().get_hz();

      this->points.push_back({x, y, z});

    }

  }

  /*-------------------------------------------------------------------------*/

};
#endif
