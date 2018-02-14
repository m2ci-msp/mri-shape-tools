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
  Json::Reader reader;
  reader.parse(settings.motion, root);

  inFile.close();

  const double tx = root["tx"].asDouble();
  const double ty = root["ty"].asDouble();
  const double tz = root["tz"].asDouble();

  Translation translation(tx, ty, tz);

  const double alpha = root["alpha"].asDouble();
  const double beta = root["beta"].asDouble();
  const double gamma = root["gamma"].asDouble();

  arma::vec origin({
      root["ox"].asDouble(),
        root["oy"].asDouble(),
        root["oz"].asDouble()
        });

  Transformation t(translation, alpha, beta, gamma);
  t.set_origin(origin);

  for(arma::vec& vertex: mesh.get_vertices() ){

    vertex = t.apply_matrix(vertex);

  }

  MeshIO::write(mesh, settings.output);

  return 0;

}
