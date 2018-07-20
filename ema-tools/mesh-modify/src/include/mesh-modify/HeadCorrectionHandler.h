#ifndef __EMA_HEAD_CORRECTION_HANDLER_H__
#define __EMA_HEAD_CORRECTION_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

class EmaHeadCorrectionHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];

    head_correct(ema, options);

  }

  void head_correct(Ema& ema, Json::Value& options) {

    const std::string left = options["left"].asString();
    const std::string right = options["right"].asString();
    const std::string front = options["front"].asString();

    ema.head_correct().with(left, right, front);

  }

};
#endif
