#ifndef __EMA_APPLY_MODIFICATIONS_H__
#define __EMA_APPLY_MODIFICATIONS_H__

#include <map>
#include <string>

#include "ema/Ema.h"

#include "Handler.h"
#include "FilterHandler.h"
#include "ResampleHandler.h"
#include "TransformHandler.h"
#include "TransformAllHandler.h"
#include "HeadCorrectionHandler.h"
#include "ReduceCoilSetHandler.h"
#include "ArithmeticHandler.h"

class ApplyModifications{

public:

  ApplyModifications(Ema& ema) : ema(ema) {

    this->handlers["filter"] = new FilterHandler();
    this->handlers["resample"] = new ResampleHandler();
    this->handlers["transform"] = new EmaTransformHandler();
    this->handlers["transform all"] = new EmaTransformAllHandler();
    this->handlers["head correction"] = new EmaHeadCorrectionHandler();
    this->handlers["reduce coil set"] = new EmaReduceCoilSetHandler();
    this->handlers["arithmetic"] = new EmaArithmeticHandler();

  }

  ~ApplyModifications() {

    for(auto& mapEntry: this->handlers) {

      delete mapEntry.second;

    }

  }

  void apply(const std::string& inputFile) {

    // try to open file
    std::ifstream inFile(inputFile);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open EMA modification file.");
    }

    Json::Value root;

    inFile >> root;
    inFile.close();

    // apply each modification to the EMA data in order of occurence
    for(Json::Value& modification: root) {

      this->handlers.at(
                        modification["action"].asString()
                        )->handle(this->ema, modification);

    }

  }

private:

  std::map<std::string, Handler*> handlers;

  Ema& ema;

};
#endif
