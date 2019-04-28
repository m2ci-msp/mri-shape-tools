#include <cfloat>

#include <json/json.h>

#include "ema/Ema.h"

#include "settings.h"

void output_json(
                 const arma::vec& front,
                 const std::string& outputFile
                 ) {

  Json::Value root(Json::arrayValue);

  Json::Value translate(Json::objectValue);

  translate["action"] = "transform all";
  translate["options"] = Json::Value(Json::objectValue);
  translate["options"]["type"] = "translate";
  translate["options"]["x"] = 0;
  translate["options"]["y"] = -front(1);
  translate["options"]["z"] = -front(2);

  root.append(translate);

  std::ofstream outFile(outputFile);

  outFile << root << std::endl;

  outFile.close();

}

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    arma::vec front;
    double min = DBL_MAX;

    for(const std::string fileName: settings.input) {

      Ema ema;
      ema.read().from(fileName);

      for(int i = 0; i < ema.info().sample_amount(); ++i) {

        const arma::vec& position = ema.coil(settings.coil).access().position(i);

        if(position(1) < min) {

          front = position;
          min = position(1);

        }

      }

    }

    output_json(front, settings.output);

    return 0;

}
