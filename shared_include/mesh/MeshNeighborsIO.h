#ifndef __MESH_NEIGHBORS_IO_H__
#define __MESH_NEIGHBORS_IO_H__

#include <fstream>

#include <json/json.h>

#include "mesh/MeshNeighbors.h"

class MeshNeighborsIO{

public:

  /*-----------------------------------------------------------------------*/

  static MeshNeighbors read(const std::string& fileName) {

    // try to open file
    std::ifstream inFile(fileName);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open landmarks file.");
    }

    // read json file
    Json::Value root;
    inFile >> root;

    inFile.close();

    MeshNeighbors meshNeighbors;

    Json::Value descriptions = root["description"];

    for( Json::Value& description: descriptions ) {
      meshNeighbors.add_description(description.asString());
    }

    Json::Value data = root["neighborhoods"];

    for( Json::Value& vertex: data ) {

      for( Json::Value& neighbor: vertex["neighbors"] ) {

        meshNeighbors.add_neighbor( vertex["vertexId"].asInt(), neighbor.asInt()) ;

      }

    }

    return meshNeighbors;

  }

  /*-----------------------------------------------------------------------*/

  static void write(
    const MeshNeighbors& meshNeighbors,
    const std::string& fileName
    ) {

    // try to open file
    std::ofstream outFile(fileName);

    auto descriptions = meshNeighbors.get_descriptions();
    auto neighborMap = meshNeighbors.get_neighbor_map();

    Json::Value jsonDescriptions(Json::arrayValue);

    for( const std::string& description: descriptions ) {

      jsonDescriptions.append(description);

    }

    Json::Value jsonNeighborList(Json::arrayValue);

    for( const auto& mapEntry: neighborMap ) {

      Json::Value jsonNeighbors(Json::arrayValue);

      for( const int& neighborId: mapEntry.second ) {
        jsonNeighbors.append(neighborId);
      }

      Json::Value entry(Json::objectValue);
      entry["vertexId"] = mapEntry.first;
      entry["neighbors"] = jsonNeighbors;

      jsonNeighborList.append(entry);

    }

    Json::Value root;

    root["description"] = jsonDescriptions;
    root["neighborhoods"] = jsonNeighborList;

    outFile << root << std::endl;

    outFile.close();

  }

  /*-----------------------------------------------------------------------*/

};

#endif
