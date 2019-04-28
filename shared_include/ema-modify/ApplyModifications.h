#ifndef __EMA_APPLY_MODIFICATIONS_H__
#define __EMA_APPLY_MODIFICATIONS_H__

#include <map>
#include <string>

#include "ema/Ema.h"

#include "ema-modify/Handler.h"
#include "ema-modify/FilterHandler.h"
#include "ema-modify/ResampleHandler.h"
#include "ema-modify/TransformHandler.h"
#include "ema-modify/TransformAllHandler.h"
#include "ema-modify/HeadCorrectionHandler.h"
#include "ema-modify/ReduceCoilSetHandler.h"
#include "ema-modify/ArithmeticHandler.h"
#include "ema-modify/RenameCoilsHandler.h"

namespace emaModify{

  class ApplyModifications{

  public:

    ApplyModifications(Ema& ema) : ema(ema) {

      this->handlers["filter"] = new FilterHandler();
      this->handlers["resample"] = new ResampleHandler();
      this->handlers["transform"] = new TransformHandler();
      this->handlers["transform all"] = new TransformAllHandler();
      this->handlers["head correction"] = new HeadCorrectionHandler();
      this->handlers["reduce coil set"] = new ReduceCoilSetHandler();
      this->handlers["arithmetic"] = new ArithmeticHandler();
      this->handlers["rename coils"] = new RenameCoilsHandler();

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

}
#endif
