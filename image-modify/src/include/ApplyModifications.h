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

class ApplyModifications{

public:

    ApplyModifications(Image& image) : image(image) {
    
        this->handlers["morphology"] = new MorphologyHandler();
        this->handlers["filter"] = new FilterHandler();
        this->handlers["crop"] = new CropHandler();
        this->handlers["histogram"] = new HistogramHandler();
        this->handlers["values"] = new ValuesHandler();
        this->handlers["segment"] = new SegmentHandler();
    
    }

    void apply(const std::string& inputFile) {
    
        // try to open file
        std::ifstream inFile(inputFile);
    
        // throw exception if file can not be opened
        if( inFile.is_open() == false ) {
          throw std::runtime_error("Cannot open image file.");
        }
    
        Json::Value root;
        Json::Reader reader;
    
        reader.parse(inFile, root);
        inFile.close();
    
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
