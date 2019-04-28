#ifndef __EMA_FILTER_HANDLER_H__
#define __EMA_FILTER_HANDLER_H__

#include <json/json.h>

#include "ema-modify/Handler.h"
#include "ema/Ema.h"

namespace emaModify{

  class FilterHandler : public Handler {

  public:

    virtual void handle(Ema& ema, Json::Value& modification) {

      Json::Value& options = modification["options"];

      const std::string type = options["type"].asString();

      if      (type == "gaussian") { gaussian(ema, options); }
      else if( type == "median" ) { median(ema, options); }

    }

    void gaussian(Ema& ema, Json::Value& options) {

      const double sigma = options["sigma"].asDouble();
      const auto& coilIds = ema.info().coil_ids();

      for(const std::string& coil: coilIds) {

        ema.coil(coil).filter().gaussian(sigma);

      }

    }

    void median(Ema& ema, Json::Value& options) {

      const int radius = options["radius"].asInt();

      const auto& coilIds = ema.info().coil_ids();

      for(const std::string& coil: coilIds) {

        ema.coil(coil).filter().median(radius);

      }

    }

  };

}
#endif
