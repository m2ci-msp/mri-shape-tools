#include <string>
#include <fstream>
#include <json/json.h>

#include "registered-ema/RegisteredEma.h"

#include "settings.h"

int get_weight_size(const std::string& fileName) {

  RegisteredEma ema;

  ema.read().from(fileName);

  return ema.access().speaker_weight(0).n_rows;

}

void output_json(const arma::vec& speakerMean, const std::string& fileName) {

  Json::Value root(Json::arrayValue);

  for(size_t i = 0; i < speakerMean.n_rows; ++i) {

    root.append(speakerMean(i));

  }

  std::ofstream outFile(fileName);

  outFile << root << std::endl;

}

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  RegisteredEma ema;

  arma::vec speakerMean = arma::zeros(get_weight_size(settings.input.at(0)));

  int counter = 0;

  for(const std::string& currentFile: settings.input ) {

    ema.read().from(currentFile);

    for(int i = 0; i < ema.info().sample_amount(); ++i) {

      speakerMean += ema.access().speaker_weight(i);
      counter += 1;

    }

  }

  speakerMean /= counter;

  output_json(speakerMean, settings.output);

  return 0;

}
