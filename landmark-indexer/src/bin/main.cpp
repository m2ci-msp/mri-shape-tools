#include <string>
#include <vector>

#include "landmark/LandmarkIO.h"
#include "mesh/MeshIO.h"

#include "settings.h"
#include "LandmarkIndexer.h"


int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh input = MeshIO::read(settings.sourceMesh);
  auto landmarks = LandmarkIO::read(settings.landmarks);

  LandmarkIndexer finder(input);
  finder.compute_indices(landmarks);

  LandmarkIO::write(landmarks, settings.output);

  return 0;

}
