#include <armadillo>
#include <json/json.h>

#include "ema/Ema.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  ema.read().from(settings.input);

  const EmaInfo& info = ema.info();

  arma::vec mean = arma::zeros(3);
  const int pointAmount = settings.coils.size() * ema.info().sample_amount();


  for(const std::string& currentCoil: settings.coils) {

    for(int i = 0; i < info.sample_amount(); ++i) {

      mean += ema.coil(currentCoil).access().position(i);

    }

  }

  mean /= pointAmount;

  Json::Value root(Json::objectValue);

  root["x"] = mean(0);
  root["y"] = mean(1);
  root["z"] = mean(2);

  std::ofstream outFile(settings.output);

  outFile << root << std::endl;

  outFile.close();

  return 0;

}
