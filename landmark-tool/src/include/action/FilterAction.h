#ifndef __FILTER_ACTION_H__
#define __FILTER_ACTION_H__

#include <sstream>
#include <json/json.h>

#include "action/Action.h"

class FilterAction : public Action {

protected:

  Json::Value add_landmarks_if_needed(const std::string& originalString) const {

    Json::Value description;

    std::stringstream(originalString) >> description;

    for(Json::Value& entry: description) {

      const std::string action = entry["action"].asString();
      Json::Value& options = entry["options"];

      // add landmarks if
      // 1. segment action is used,
      // 2. "with landmarks" type is wanted,
      // 3. and landmarks are not present in filter string
      Json::Value nullValue;

      if( action == "segment" && options["type"].asString() == "with landmarks" && options["landmarks"] == nullValue){

        Json::Value landmarks(Json::arrayValue);

        for(const auto& mark: LandmarkPool::get_instance()->get_all_landmarks() ) {

          Json::Value landmark(Json::objectValue);

          landmark["x"] = Json::Value(mark->get_position().get_canonical_x());
          landmark["y"] = Json::Value(mark->get_position().get_canonical_y());
          landmark["z"] = Json::Value(mark->get_position().get_canonical_z());

          landmarks.append(landmark);

        }

        options["landmarks"] = landmarks;

      }

    }

    return description;

  }

};
#endif
