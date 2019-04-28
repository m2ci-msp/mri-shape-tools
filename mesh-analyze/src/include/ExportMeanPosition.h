#ifndef __EXPORT_MEAN_POSITION_H__
#define __EXPORT_MEAN_POSITION_H__

#include <fstream>

#include <json/json.h>
#include "mesh/Mesh.h"

class ExportMeanPosition{

public:

  ExportMeanPosition(const Mesh& mesh) :

    mesh(mesh) {

  }

  void to(const std::string& fileName) const {

    std::ofstream outFile(fileName);

    arma::vec mean = mesh.get_center();

    Json::Value jsonPosition(Json::objectValue);

    jsonPosition["x"] = mean(0);
    jsonPosition["y"] = mean(1);
    jsonPosition["z"] = mean(2);

    outFile << jsonPosition << std::endl;

    outFile.close();

  }

private:

  const Mesh& mesh;

};
#endif
