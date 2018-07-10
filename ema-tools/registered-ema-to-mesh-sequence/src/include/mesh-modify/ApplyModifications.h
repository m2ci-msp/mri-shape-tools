#ifndef __APPLY_MODIFICATIONS_H__
#define __APPLY_MODIFICATIONS_H__

#include <map>
#include <string>
#include <sstream>

#include <json/json.h>

#include "mesh/Mesh.h"

#include "Handler.h"
#include "TranslateHandler.h"
#include "RotateHandler.h"
#include "MapToCoordinateSystemHandler.h"
#include "MapFromCoordinateSystemHandler.h"

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

  }

  ~ApplyModifications() {

    for(auto& handler: this->handlers) {

      delete handler.second;

    }

  }

  void apply(const std::string& jsonString) {

    Json::Value root;

    std::stringstream(jsonString) >> root;

    // apply each modification to the mesh in order of occurence
    for(Json::Value& modification: root) {

      this->handlers.at(
                        modification["action"].asString()
                        )->handle(this->mesh, modification);

    }

  }

};
#endif
