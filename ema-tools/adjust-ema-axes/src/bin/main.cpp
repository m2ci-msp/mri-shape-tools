#include <json/json.h>

#include "ema/Ema.h"
#include "ema/EmaCoordinateSystem.h"
#include "mesh/Mesh.h"


#include "settings.h"
#include "AxisAnalysis.h"

void output_json(
                 const Mesh& pointCloud,
                 EmaCoordinateSystem& system,
                 const std::string& outputFile
                 ) {

  Json::Value root(Json::arrayValue);

  Json::Value translateBack(Json::objectValue);
  Json::Value translateBackOptions(Json::objectValue);

  Json::Value mapToSystem(Json::objectValue);

  const arma::vec center = pointCloud.get_center();

  mapToSystem["action"] = "transform";
  mapToSystem["options"] = system.get_json();
  mapToSystem["type"] = "map to coordinate system";

  translateBack["action"] = "transform";
  translateBack["options"] = Json::Value(Json::objectValue);
  translateBack["options"]["x"] = center(0);
  translateBack["options"]["y"] = center(1);
  translateBack["options"]["z"] = center(2);
  translateBack["options"]["type"] = "translate";

  root.append(mapToSystem);
  root.append(translateBack);

  std::ofstream outFile(outputFile);

  outFile << root << std::endl;

  outFile.close();

}

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Ema ema;
    ema.read().from(settings.input);

    Mesh midsagittalCloud = ema.point_cloud().from_all_time_frames(settings.midsagittalCoils);

    AxisAnalysis analysis(midsagittalCloud);

    EmaCoordinateSystem system = analysis.get_data_aligned_coordinate_system();

    output_json(midsagittalCloud, system, settings.output);

    return 0;

}
