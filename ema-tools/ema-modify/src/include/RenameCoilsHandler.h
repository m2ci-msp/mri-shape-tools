#ifndef __EMA_RENAME_COILS_HANDLER_H__
#define __EMA_RENAME_COILS_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

class EmaRenameCoilsHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];

    rename_coils(ema, options);

  }

private:

  void rename_coils(Ema& ema, Json::Value& options) {

    std::map<std::string, std::string> mapping;

    for(const Json::Value& entry: options["mapping"] ) {

      mapping[entry["from"].asString()] = entry["to"].asString();

    }

    ema.rename_coils().with(mapping);

  }

};
#endif
