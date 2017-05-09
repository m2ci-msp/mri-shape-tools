#include <string>
#include <vector>

#include "settings.h"
#include "CloudPurger.h"
#include "CloudBouncer.h"

#include "landmark/LandmarkIO.h"
#include "mesh/MeshIO.h"
#include "mesh/NormalEstimation.h"


int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh input = MeshIO::read(settings.source);

  if( settings.landmarksPresent == true) {
    auto landmarks = LandmarkIO::read(settings.landmarks);


    CloudPurger purger(input, landmarks);

    input = purger.purge();

  }

  if( settings.boundaryMeshPresent == true ) {

    Mesh boundary = MeshIO::read(settings.boundaryMesh);
    NormalEstimation normals(boundary);
    boundary.set_vertex_normals(normals.compute());
    CloudBouncer bouncer(boundary);
    bouncer.set_keep_points_below(settings.keepPointsBelow) \
      .set_maximum_distance(settings.maxDistance)\
      .set_bounce_sides(settings.bounceSides) \
      .set_side_distance(settings.sideDistance);

    input = bouncer.bounce(input);

  }

  MeshIO::write(input, settings.output);

  return 0;

}
