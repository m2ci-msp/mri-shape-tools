#include <json/json.h>

#include "ema/Ema.h"
#include "mesh/Mesh.h"
#include "mesh/MeshCombiner.h"

#include "settings.h"
#include "MidsagittalRotation.h"

void output_json(
                 const arma::vec& origin,
                 const double& angle,
                 const std::string& outputFile
                 ) {

  Json::Value root(Json::arrayValue);

  Json::Value rotate(Json::objectValue);

  rotate["action"] = "transform all";
  rotate["options"] = Json::Value(Json::objectValue);
  rotate["options"]["origin"] = Json::Value(Json::arrayValue);
  rotate["options"]["origin"].append(origin(0));
  rotate["options"]["origin"].append(origin(1));
  rotate["options"]["origin"].append(origin(2));
  rotate["options"]["angle"] = angle;
  rotate["options"]["angleUnit"] = "degrees";
  rotate["options"]["type"] = "rotate";
  rotate["options"]["axis"] = "z";

  root.append(rotate);

  std::ofstream outFile(outputFile);

  outFile << root << std::endl;

  outFile.close();

}

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Mesh midsagittalCloud;

    for(const std::string fileName: settings.input) {

      Ema ema;
      ema.read().from(fileName);

      Mesh current = ema.point_cloud().from_all_time_frames(settings.midsagittalCoils);

      midsagittalCloud = MeshCombiner::combine(midsagittalCloud, current);

    }

    MidsagittalRotation midsagittalRotation(midsagittalCloud);

    double angle;
    arma::vec origin;

    midsagittalRotation.estimate(origin, angle);

    output_json(origin, angle, settings.output);

    return 0;

}
