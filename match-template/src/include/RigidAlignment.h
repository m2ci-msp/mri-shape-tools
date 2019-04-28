#ifndef __RIGID_ALIGNMENT_H__
#define __RIGID_ALIGNMENT_H__

#include "settings.h"
#include "mesh/Mesh.h"
#include "mesh/NormalEstimation.h"
#include "landmark/LandmarkIO.h"

#include "optimization/rigidalignment/Energy.h"
#include "optimization/rigidalignment/EnergyMinimizer.h"
#include "optimization/rigidalignment/MinimizerSettings.h"

class RigidAlignment{

private:

  Mesh source;
  Mesh target;
  Settings settings;

public:

  RigidAlignment(
                 const Mesh& source,
                 const Mesh& target,
                 const Settings& settings
                 ) : source(source), target(target), settings(settings) {

  }

  Mesh perform() {

    // deal with target meshes that do not provide normals
    if( this->target.has_normals() == false ) {

      // try to estimate normals if the mesh has faces
      if( this->target.has_faces() == true ) {

        NormalEstimation estimation(target);
        this->target.set_vertex_normals(estimation.compute());

      } // end if
      else {

        // use point-to-point distance measure
        this->settings.rigidAlignmentEnergySettings.searchStrategy =
          rigidAlignment::EnergySettings::SearchStrategy::BASIC;

      } // end else

    } // end if


    std::vector<Landmark> landmarks;

    if(settings.landmarksPresent == true) {

      landmarks = LandmarkIO::read(settings.landmarks);

    }


    // initialize with landmarks
    if( this->settings.landmarksPresent == true) {

      // use scopes to avoid name clashes
      {

        rigidAlignment::EnergyData data(source, target);
        data.landmarks = landmarks;

        this->settings.rigidAlignmentEnergySettings.weights["dataTerm"] = 0;
        this->settings.rigidAlignmentEnergySettings.weights["landmarkTerm"] = 1;

        // do not optimize rotation: might drastically shrink the template
        const bool noScaling = this->settings.rigidAlignmentMinimizerSettings.noScaling;

        // only use one iteration: landmarks do not change between iterations
        const int iterationAmount = this->settings.rigidAlignmentMinimizerSettings.iterationAmount;

        this->settings.rigidAlignmentMinimizerSettings.noScaling = true;
        this->settings.rigidAlignmentMinimizerSettings.iterationAmount = 1;

        rigidAlignment::Energy energy(data, settings.rigidAlignmentEnergySettings);
        rigidAlignment::EnergyMinimizer minimizer(energy, settings.rigidAlignmentMinimizerSettings);

        minimizer.minimize();

        // replace source with aligned one
        this->source = energy.derived_data().source;

        // restore original settings
        this->settings.rigidAlignmentMinimizerSettings.iterationAmount = iterationAmount;
        this->settings.rigidAlignmentMinimizerSettings.noScaling = noScaling;

      }

      {
        // perform combined optimization
        rigidAlignment::EnergyData data(source, target);
        data.landmarks = landmarks;

        this->settings.rigidAlignmentEnergySettings.weights["dataTerm"] = 1;
        this->settings.rigidAlignmentEnergySettings.weights["landmarkTerm"] = 1;

        rigidAlignment::Energy energy(data, settings.rigidAlignmentEnergySettings);

        rigidAlignment::EnergyMinimizer minimizer(energy, settings.rigidAlignmentMinimizerSettings);

        minimizer.minimize();

        // replace source with aligned one
        this->source = energy.derived_data().source;

      }

    }
    else {

      // only use data
      rigidAlignment::EnergyData data(source, target);

      this->settings.rigidAlignmentEnergySettings.weights["dataTerm"] = 1;
      this->settings.rigidAlignmentEnergySettings.weights["landmarkTerm"] = 0;

      rigidAlignment::Energy energy(data, settings.rigidAlignmentEnergySettings);

      rigidAlignment::EnergyMinimizer minimizer(energy, settings.rigidAlignmentMinimizerSettings);

      minimizer.minimize();

      // replace source with aligned one
      this->source = energy.derived_data().source;

    }

    return this->source;

  }

};
#endif
