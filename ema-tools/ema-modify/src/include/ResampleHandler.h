#ifndef __RESAMPLE_HANDLER_H__
#define __RESAMPLE_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

class ResampleHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];
    resample(ema, options);

  }

  void resample(Ema& ema, Json::Value& options) {

    const double samplingRate = options["samplingRate"].asDouble();
    ema.resample().to(samplingRate);

  }

};
#endif
