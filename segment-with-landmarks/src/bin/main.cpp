#include <fstream>
#include <json/json.h>

#include "settings.h"
#include "image/Image.h"
#include "mesh/MeshIO.h"

#include "CreatePointCloud.h"

std::vector<arma::vec> read_landmarks(const std::string& fileName) {

  std::vector<arma::vec> landmarks;


  // try to open file
  std::ifstream inFile(fileName);

  // throw exception if file can not be opened
  if( inFile.is_open() == false ) {
    throw std::runtime_error("Cannot open landmarks file.");
  }

  // read json file
  Json::Value root;
  Json::Reader reader;

  reader.parse(inFile, root);

  inFile.close();

  for(const Json::Value& mark: root) {

    const double x = mark["x"].asDouble();
    const double y = mark["y"].asDouble();
    const double z = mark["z"].asDouble();

    landmarks.push_back({x, y, z});

  }

  return landmarks;

}

void normalize_landmarks(std::vector<arma::vec>& landmarks, const Image& image) {

  arma::vec origin({

        image.info().get_origin_x() / image.info().get_hx(),
        image.info().get_origin_y() / image.info().get_hy(),
        image.info().get_origin_z() / image.info().get_hz()

    });

  for(arma::vec& mark: landmarks) {

    mark -= origin;

  }

}

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image source;

  source.read().from(settings.source);

  std::vector<arma::vec> landmarks = read_landmarks(settings.landmarkFile);

  normalize_landmarks(landmarks, source);

  source.segment().with_landmarks(landmarks);

  source.write().to(settings.output);

  // output point cloud if wanted
  if(settings.createCloud == true) {

    CreatePointCloud cloud(source);
    cloud.set_flip(settings.flip);

    Mesh pointCloud = cloud.create_point_cloud();
    MeshIO::write(pointCloud, settings.outputCloud);

  }

  return 0;

}
