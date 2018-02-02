#ifndef __APPLY_MODIFICATIONS_H__
#define __APPLY_MODIFICATIONS_H__

#include <map>
#include <string>

#include "image/Image.h"

#include "Handler.h"
#include "MorphologyHandler.h"
#include "FilterHandler.h"
#include "CropHandler.h"
#include "HistogramHandler.h"
#include "ValuesHandler.h"
#include "SegmentHandler.h"
#include "EdgeHandler.h"
#include "InterpolateSliceHandler.h"

class ApplyModifications{

 public:

 ApplyModifications(Image& image) : image(image) {
    
    this->handlers["morphology"] = new MorphologyHandler();
    this->handlers["filter"] = new FilterHandler();
    this->handlers["crop"] = new CropHandler();
    this->handlers["histogram"] = new HistogramHandler();
    this->handlers["values"] = new ValuesHandler();
    this->handlers["segment"] = new SegmentHandler();
    this->handlers["edge"] = new EdgeHandler();
    this->handlers["interpolateSlice"] = new InterpolateSliceHandler();
    
  }

  ~ApplyModifications() {

    for(auto& handler: this->handlers) {

      delete handler.second;

    }

  }

  void apply(const std::string& jsonString) {
    
    Json::Value root;
    Json::Reader reader;
    
    reader.parse(jsonString, root);
    
    // apply each modification to the image in order of occurence
    for(Json::Value& modification: root) {
    
      this->handlers.at(
                        modification["action"].asString()
                        )->handle(this->image, modification);
    
    }
    
  }

 private:

  std::map<std::string, Handler*> handlers;

  Image& image;

};
#endif
