#ifndef __IMAGE_WRITER_H__
#define __IMAGE_WRITER_H__

#include <fstream>
#include <json/json.h>

#include "image/ImageBoundary.h"
#include "image/ImageData.h"

class ImageWriter{

public:

  ImageWriter(const ImageData& imageData) : imageData(imageData) {
  }

  void to(const std::string& fileName) {

    // create image data without boundary information
    ImageData copy = this->imageData;
    ImageBoundary(copy).change_boundary(0, 0, 0);

    // try to open file
    std::ofstream outFile(fileName);

    // throw exception if file can not be opened
    if( outFile.is_open() == false ) {
      throw std::runtime_error("Cannot open image file.");
    }

    // create root node of json file
    Json::Value root(Json::objectValue);

    // add meta data

    // dimensions of the scan
    Json::Value dimensions(Json::objectValue);

    dimensions["x"] = copy.nx;
    dimensions["y"] = copy.ny;
    dimensions["z"] = copy.nz;

    root["dimensions"] = dimensions;

    // origin of scan
    Json::Value origin(Json::objectValue);
    origin["x"] = copy.originX;
    origin["y"] = copy.originY;
    origin["z"] = copy.originZ;

    root["origin"] = origin;

    // spacings along the grid axes
    Json::Value spacings(Json::objectValue);

    spacings["x"] = copy.hx;
    spacings["y"] = copy.hy;
    spacings["z"] = copy.hz;

    root["spacings"] = spacings;

    // add data
    Json::Value data(Json::arrayValue);

    for( const double& value: copy.values ) {
      data.append(value);
    }

    root["data"] = data;

    Json::StyledStreamWriter writer;
    writer.write(outFile, root);

  }


private:

  const ImageData& imageData;

};

#endif
