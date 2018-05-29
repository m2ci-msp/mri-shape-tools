#ifndef __EMA_REDUCE_COIL_SET_HANDLER_H__
#define __EMA_REDUCE_COIL_SET_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

class EmaReduceCoilSetHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];

    reduce_coil_set(ema, options);

  }

  void reduce_coil_set(Ema& ema, Json::Value& options) {

    std::vector<std::string> coilNames;

    for(Json::Value& coil: options["coils"]) {

      coilNames.push_back(coil.asString());

    }

    ema.reduce_coil_set().to(coilNames);

  }

};
#endif
