#include <string>
#include <vector>

#include "mesh/MeshIO.h"
#include "scan/ScanIO.h"

#include "visualize/ProjectMesh.h"

#include "settings.h"


int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh mesh = MeshIO::read(settings.mesh);
  Scan scan = ScanIO::read_from(settings.scan);

  // enhance contrast and rescale scan values
  // discard 0.25% of darkest and brightest values
  scan.transform()->discard_values(0.25, 0.25);
  scan.transform()->scale_values(0, 255);

  ProjectMesh projection(mesh, scan, 5);

  scan = projection.get_projected_mesh();

  ScanIO::write_to(settings.output, scan);

  return 0;

}
