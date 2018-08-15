#ifndef __EXPORT_BOUNDING_BOX_H__
#define __EXPORT_BOUNDING_BOX_H__

#include <fstream>

#include <json/json.h>
#include "mesh/Mesh.h"

class ExportBoundingBox{

public:

  ExportBoundingBox(const Mesh& mesh) :

    mesh(mesh) {

  }

  void to(const std::string& fileName) const {

    std::ofstream outFile(fileName);

    arma::vec min, max;

    mesh.get_bounding_box(min, max);

    Json::Value minPosition(Json::objectValue);

    minPosition["x"] = min(0);
    minPosition["y"] = min(1);
    minPosition["z"] = min(2);

    Json::Value maxPosition(Json::objectValue);

    maxPosition["x"] = max(0);
    maxPosition["y"] = max(1);
    maxPosition["z"] = max(2);

    Json::Value root(Json::objectValue);

    root["min"] = minPosition;
    root["max"] = maxPosition;

    outFile << root << std::endl;

    outFile.close();

  }

private:

  const Mesh& mesh;

};
#endif
