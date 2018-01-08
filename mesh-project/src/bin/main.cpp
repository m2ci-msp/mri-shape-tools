#include <string>
#include <vector>

#include "mesh/MeshIO.h"
#include "image/Image.h"

#include "visualize/ProjectMesh.h"

#include "settings.h"


int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh mesh = MeshIO::read(settings.mesh);
  Image scan;
  scan.read().from(settings.scan);

  // enhance contrast and rescale scan values
  // discard 0.25% of darkest and brightest values
  scan.values().discard(0.25, 0.25);
  scan.values().scale(0, 255);

  ProjectMesh projection(mesh, scan, 5);

  scan = projection.get_projected_mesh();

  scan.write().to(settings.output);

  return 0;

}
