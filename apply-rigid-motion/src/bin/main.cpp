#include <fstream>
#include <json/json.h>

#include "mesh/MeshIO.h"
#include "matrix/Transformation.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh mesh = MeshIO::read(settings.input);

  std::ifstream inFile(settings.motion);
  Json::Value root;

  inFile >> root;

  inFile.close();

  const double tx = root["tx"].asDouble();
  const double ty = root["ty"].asDouble();
  const double tz = root["tz"].asDouble();

  Translation translation(tx, ty, tz);

  // angles are not const: they may change depending on the angle unit
  double alpha = root["alpha"].asDouble();
  double beta = root["beta"].asDouble();
  double gamma = root["gamma"].asDouble();

  // check if the angle unit is provided
  if( root["angleUnit"].empty() == false) {

    // derive radians from degrees
    if( root["angleUnit"].asString() == "degrees") {

      alpha = alpha / 180 * M_PI;
      beta = beta / 180 * M_PI;
      gamma = gamma / 180 * M_PI;

    }

  }

  // the rotation center is optional
  arma::vec origin;

  if( root["ox"].empty() ||
      root["oy"].empty() ||
      root["oz"].empty()
      ) {

    // if it is omitted, use the mesh center
    origin = mesh.get_center();

  }
  else {

    // otherwise the provided one
    origin =  arma::vec({
          root["ox"].asDouble(),
          root["oy"].asDouble(),
          root["oz"].asDouble()
          });

  }

  Transformation t(translation, alpha, beta, gamma);
  t.set_origin(origin);

  for(arma::vec& vertex: mesh.get_vertices() ){

    vertex = t.apply_matrix(vertex);

  }

  MeshIO::write(mesh, settings.output);

  return 0;

}
