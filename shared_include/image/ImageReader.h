#ifndef __IMAGE_READER_H__
#define __IMAGE_READER_H__

#include <string>
#include <fstream>
#include <json/json.h>

#include "image/ImageData.h"

class ImageReader{

public:

  ImageReader(ImageData& imageData) : imageData(imageData) {
  }

  void from(const std::string fileName) {

    // try to open file
    std::ifstream inFile(fileName);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open image file.");
    }

    Json::Value root;
    Json::Reader reader;

    reader.parse(inFile, root);
    inFile.close();

    // set boundary size to 0
    this->imageData.bx = 0;
    this->imageData.by = 0;
    this->imageData.bz = 0;

    // read origin
    Json::Value origin = root["origin"];
    this->imageData.orgX = origin["x"].asDouble();
    this->imageData.orgY = origin["y"].asDouble();
    this->imageData.orgZ = origin["z"].asDouble();

    // read dimensions
    Json::Value dimensions = root["dimensions"];
    this->imageData.nx = dimensions["x"].asInt();
    this->imageData.ny = dimensions["y"].asInt();
    this->imageData.nz = dimensions["z"].asInt();

    // read spacings
    Json::Value spacings = root["spacings"];
    this->imageData.hx = spacings["x"].asDouble();
    this->imageData.hy = spacings["y"].asDouble();
    this->imageData.hz = spacings["z"].asDouble();

    // read data
    this->imageData.values.clear();

    Json::Value values = root["data"];

    for( Json::Value& value: values) {
      this->imageData.values.push_back(value.asDouble());
    }

  }


private:

  ImageData& imageData;

};

#endif
