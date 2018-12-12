#include <armadillo>

#include "settings.h"
#include "RigidAlignment.h"

#include "landmark/LandmarkIO.h"

#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "mesh/MeshNeighbors.h"
#include "mesh/MeshNeighborsIO.h"
#include "mesh/MeshOneRingNeighborsBuilder.h"
#include "mesh/MeshGeodesicNeighborsBuilder.h"
#include "mesh/MeshSphericalNeighborsBuilder.h"
#include "mesh/MeshResolution.h"

#include "mesh/MeshSmooth.h"

#include "optimization/matchtemplate/Energy.h"
#include "optimization/matchtemplate/EnergyMinimizer.h"
#include "optimization/matchtemplate/MinimizerSettings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  // read input data
  Mesh source = MeshIO::read(settings.source);
  Mesh target = MeshIO::read(settings.target);

  if( settings.performRigidAlignment == true) {

    source = RigidAlignment(source, target, settings).perform();

  }

  // deal with target meshes that do not provide normals
  if( settings.fixedNeighbors == false && target.has_normals() == false ) {

    // try to estimate normals if the mesh has faces
    if( target.has_faces() == true ) {
      NormalEstimation estimation(target);
      target.set_vertex_normals(estimation.compute());
    } // end if
    else {
      // use point-to-point distance measure
      settings.energySettings.searchStrategy =
        matchTemplate::EnergySettings::SearchStrategy::BASIC;
    } // end else

  } // end if

  MeshResolution resolution(source);

  arma::vec min, max;

  source.get_bounding_box(min, max);

  const double scaleFactor = 1. / arma::norm( max - min );

  settings.energySettings.maxDistance  *= scaleFactor;
  settings.energySettings.searchRadius *= scaleFactor;

  MeshNeighbors neighbors;

  if( settings.meshNeighborhoodPresent == true ) {
    neighbors = MeshNeighborsIO::read(settings.meshNeighborhood);
  }
  else {
    MeshOneRingNeighborsBuilder builder(source);
    neighbors = neighbors + builder.get_neighbors();

    if( settings.addGeodesic == true ) {

      MeshGeodesicNeighborsBuilder builder(
        source, resolution.get_resolution() * settings.geodesicNeighborhoodSize);
      neighbors = neighbors + builder.get_neighbors();

    }

    if( settings.addSpherical == true ) {

      MeshSphericalNeighborsBuilder builder(
        source, resolution.get_resolution() * settings.sphericalNeighborhoodSize);
      neighbors = neighbors + builder.get_neighbors();

    }
  }

  for( arma::vec& vertex: source.get_vertices() ) {
    vertex *= scaleFactor;
  }

  for( arma::vec& vertex: target.get_vertices() ) {
    vertex *= scaleFactor;
  }

  matchTemplate::EnergyData data(source, neighbors, target);

  if(settings.landmarksPresent == true) {
    data.landmarks = LandmarkIO::read(settings.landmarks);

    for( Landmark& mark: data.landmarks ) {

      mark.targetPosition *= scaleFactor;

    }

  }

  settings.energySettings.weights["dataTerm"] = 1;
  settings.energySettings.weights["smoothnessTerm"] = settings.smoothnessTermWeight;
  settings.energySettings.weights["postSmoothnessTerm"] = settings.postSmoothnessTermWeight;
  settings.energySettings.changeFactors["smoothnessTerm"] = settings.smoothnessTermChange;
  settings.energySettings.weights["landmarkTerm"] = settings.landmarkTermWeight;
  settings.energySettings.changeFactors["landmarkTerm"] = settings.landmarkTermChange;

  matchTemplate::Energy energy(data, settings.energySettings);

  matchTemplate::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  for( arma::vec& vertex: energy.derived_data().source.get_vertices() ) {
    vertex /= scaleFactor;
  }

  Mesh result = energy.derived_data().source;

  // remove normals
  result.get_vertex_normals().clear();

  MeshSmooth(result).apply(settings.meshSmoothIterations);

  MeshIO::write(result, settings.output);

  return 0;

}
