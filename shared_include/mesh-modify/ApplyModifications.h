#ifndef __MESH_APPLY_MODIFICATIONS_H__
#define __MESH_APPLY_MODIFICATIONS_H__

#include <map>
#include <string>
#include <sstream>

#include <json/json.h>

#include "mesh/Mesh.h"

#include "mesh-modify/Handler.h"
#include "mesh-modify/TranslateHandler.h"
#include "mesh-modify/RotateHandler.h"
#include "mesh-modify/MapToCoordinateSystemHandler.h"
#include "mesh-modify/MapFromCoordinateSystemHandler.h"
#include "mesh-modify/ScaleHandler.h"

namespace meshModify{

  class ApplyModifications{

  private:

    std::map<std::string, Handler*> handlers;

    Mesh& mesh;

  public:

    ApplyModifications(Mesh& mesh) : mesh(mesh) {

      this->handlers["translate"] = new TranslateHandler();
      this->handlers["rotate"] = new RotateHandler();
      this->handlers["map to coordinate system"] = new MapToCoordinateSystemHandler();
      this->handlers["map from coordinate system"] = new MapFromCoordinateSystemHandler();
      this->handlers["scale"] = new ScaleHandler();

    }

    ~ApplyModifications() {

      for(auto& handler: this->handlers) {

        delete handler.second;

      }

    }

    void apply(const Json::Value& modifications) {

      // apply each modification to the mesh in order of occurence
      for(const Json::Value& modification: modifications) {

        this->handlers.at(
                          modification["action"].asString()
                          )->handle(this->mesh, modification);

      }

    }

    void apply(const std::string& inputFile) {

      // try to open file
      std::ifstream inFile(inputFile);

      // throw exception if file can not be opened
      if( inFile.is_open() == false ) {

        throw std::runtime_error("Cannot open mesh modification file.");

      }

      Json::Value root;

      inFile >> root;
      inFile.close();

      apply(root);

    }

  };

}
#endif
