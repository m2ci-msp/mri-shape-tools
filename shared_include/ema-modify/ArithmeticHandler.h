#ifndef __EMA_ARITHMETIC_HANDLER_H__
#define __EMA_ARITHMETIC_HANDLER_H__

#include <json/json.h>

#include "ema-modify/Handler.h"
#include "ema/Ema.h"

namespace emaModify{

  class ArithmeticHandler : public Handler {

  public:

    virtual void handle(Ema& ema, Json::Value& modification) {

      Json::Value& options = modification["options"];

      const std::string& type = options["type"].asString();

      if      ( type == "plus" ) { plus(ema, options); }
      else if ( type == "minus" ) { minus(ema, options); }

    }

    void plus(Ema& ema, Json::Value& options) {

      const std::string left = options["left"].asString();
      const std::string right = options["right"].asString();

      ema.coil(left).arithmetic().plus(ema.coil(right).access());

    }

    void minus(Ema& ema, Json::Value& options) {

      const std::string left = options["left"].asString();
      const std::string right = options["right"].asString();

      ema.coil(left).arithmetic().minus(ema.coil(right).access());

    }

  };

}
#endif
