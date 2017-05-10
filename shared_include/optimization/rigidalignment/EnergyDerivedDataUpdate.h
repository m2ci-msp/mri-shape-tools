#ifndef __RIGID_ALIGNMENT_ENERGY_DERIVED_DATA_UPDATE_H__
#define __RIGID_ALIGNMENT_ENERGY_DERIVED_DATA_UPDATE_H__

#include "mesh/NormalEstimation.h"

#include "optimization/rigidalignment/EnergyData.h"
#include "optimization/rigidalignment/EnergyDerivedData.h"
#include "optimization/rigidalignment/EnergySettings.h"

namespace rigidAlignment{

  /* class that computes the derived energy data in EnergyDerivedData */
  class EnergyDerivedDataUpdate {

  public:

    /*--------------------------------------------------------------------------*/

    EnergyDerivedDataUpdate(
      EnergyData& energyData,
      EnergyDerivedData& energyDerivedData,
      EnergySettings& energySettings
      ) :
      energyData(energyData),
      energyDerivedData(energyDerivedData),
      energySettings(energySettings) {
    }

    /*--------------------------------------------------------------------------*/

    /* updates normals of the source mesh that depend on the current transformations */
    void source_normals() {

      NormalEstimation estimator(this->energyDerivedData.source);
      this->energyDerivedData.source.set_vertex_normals(estimator.compute());

    }

    /*--------------------------------------------------------------------------*/

    /* updates the vertices of the template mesh and the transformation */
    void for_transformation() {

      transform_source();

      linearize_source();

      linearize_landmark_source();

    }

    /*--------------------------------------------------------------------------*/

    /* updates data structures that depend on the neighbor correspondences */
    void for_neighbors() {

      linearize_source_and_target();

      // update data term weight
      const double weight = this->energySettings.weights.at("dataTerm");

      this->energyDerivedData.weights["dataTerm"] = weight /
        ( ( this->energyDerivedData.sourceIndices.size() == 0)?
          1. : this->energyDerivedData.sourceIndices.size() );

    }

    /*--------------------------------------------------------------------------*/

    /* update data structures that depend on the landmarks */
    void for_landmarks() {

      setup_landmark_indicators();

      linearize_landmark_source();
      linearize_landmark_target();

      // update landmark term weight
      const double weight =
        this->energySettings.weights.at("landmarkTerm");

      this->energyDerivedData.weights["landmarkTerm"] = weight /
        ( ( this->energyData.landmarks.size() == 0)?
          1. : this->energyData.landmarks.size() );

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    /* transforms original template according to the current transformation
     * parameters
     */
    void transform_source() {

      const std::vector<arma::vec>& original = this->energyData.source.get_vertices();
      const Transformation& transformation = this->energyData.transformationMatrix;

      std::vector<arma::vec>& changed = this->energyDerivedData.source.get_vertices();

      const int vertexAmount = original.size();

      for(int i = 0; i < vertexAmount; ++i) {

        changed[i] = transformation.apply_matrix(original[i]);

      }

    }

    /*--------------------------------------------------------------------------*/

    /* linearize vertices of the source mesh that are present in the
     * neighbor correspondences, set all other values to 0
     */
    void linearize_source() {

      // setup helper variables for accessing the necessary data
      const std::vector<arma::vec>& sourceVertices =
        this->energyDerivedData.source.get_vertices();

      const std::vector<int>& sourceIndices =
        this->energyDerivedData.sourceIndices;

      arma::vec& linearizedSource = this->energyDerivedData.linearizedSource;

      // setup end

      linearizedSource = arma::vec(3 * sourceVertices.size(), arma::fill::zeros);

      for(unsigned int i = 0; i < sourceIndices.size(); ++i) {

        const int index = sourceIndices.at(i);
        const arma::vec& point = sourceVertices.at(index);

        for(int j = 0; j < 3; ++j) {

          linearizedSource(3 * index + j) = point(j);

        }

      }

    }

    /*--------------------------------------------------------------------------*/

    /* linearize corresponding vertices of the source and target meshes that are
     * present in the neighbor correspondences, set all other values to 0
     *
     * if required compute the projection onto the normal plane of the
     * corresponding target vertex
     */
    void linearize_source_and_target() {

      // setup helper variables for accessing the necessary data
      const std::vector<arma::vec>& sourceVertices =
        this->energyDerivedData.source.get_vertices();

      const std::vector<arma::vec>& targetVertices =
        this->energyData.target.get_vertices();

      const std::vector<int>& sourceIndices =
        this->energyDerivedData.sourceIndices;

      const std::vector<int>& targetIndices =
        this->energyDerivedData.targetIndices;

      arma::vec& linearizedSource = this->energyDerivedData.linearizedSource;
      arma::vec& linearizedTarget = this->energyDerivedData.linearizedTarget;

      // setup end

      linearizedSource =
        arma::vec(3 * sourceVertices.size(), arma::fill::zeros);

      linearizedTarget =
        arma::vec(3 * sourceVertices.size(), arma::fill::zeros);

      for(unsigned int i = 0; i < sourceIndices.size(); ++i) {

        const int sourceIndex = sourceIndices.at(i);
        const int targetIndex = targetIndices.at(i);

        const arma::vec& sourcePoint = sourceVertices.at(sourceIndex);

        // target point is no const reference, it might change
        arma::vec targetPoint = targetVertices.at(targetIndex);

        // check if we are using the projection onto the normal plane
        // this is deactivated if fixed correspondences are used
        if(
          this->energySettings.useProjection &&
          this->energyData.target.has_normals() &&
          this->energySettings.searchStrategy != EnergySettings::SearchStrategy::FIXED
          ) {

          const arma::vec& targetNormal =
            this->energyData.target.get_vertex_normals().at(targetIndex);

          // compute the projection point and use it as new target point
          targetPoint = sourcePoint + arma::dot(
            targetPoint - sourcePoint, targetNormal) * targetNormal;

        } // end if use_projection

        for(int j = 0; j < 3; ++j) {

          linearizedSource(3 * sourceIndex + j) = sourcePoint(j);

          linearizedTarget(3 * sourceIndex + j) = targetPoint(j);

        } // end for j

      } // end for i

    } // end linearize_source_and_target

    /*--------------------------------------------------------------------------*/

    void setup_landmark_indicators() {


      // clear current indicators and set to false
      const int vertexAmount =
        this->energyDerivedData.source.get_vertices().size();
      this->energyDerivedData.isLandmark.clear();
      this->energyDerivedData.isLandmark.resize(vertexAmount);

      for(const Landmark& landmark: this->energyData.landmarks) {
        this->energyDerivedData.isLandmark.at(landmark.sourceIndex) = true;
      } // end for landmark


    } // end setup_landmark_indicators

    /*--------------------------------------------------------------------------*/

    void linearize_landmark_source() {

      // setup helper variables for accessing the necessary data
      const std::vector<arma::vec>& sourceVertices =
        this->energyDerivedData.source.get_vertices();

      const std::vector<Landmark>& landmarks =
        this->energyData.landmarks;

      arma::vec& linearizedLandmarkSource =
        this->energyDerivedData.linearizedLandmarkSource;

      // setup end

      linearizedLandmarkSource = arma::vec(3 * sourceVertices.size(), arma::fill::zeros);

      for(unsigned int i = 0; i < landmarks.size(); ++i) {

        const int index = landmarks.at(i).sourceIndex;
        const arma::vec& point = sourceVertices.at(index);

        for(int j = 0; j < 3; ++j) {

          linearizedLandmarkSource(3 * index + j) = point(j);

        } // end for j

      } // end for i

    } // end linearize_landmark_source

    /*--------------------------------------------------------------------------*/

    void linearize_landmark_target() {

      // setup helper variables for accessing the necessary data
      const std::vector<arma::vec>& sourceVertices =
        this->energyDerivedData.source.get_vertices();

      const std::vector<Landmark>& landmarks =
        this->energyData.landmarks;

      arma::vec& linearizedLandmarkTarget =
        this->energyDerivedData.linearizedLandmarkTarget;

      // setup end

      linearizedLandmarkTarget = arma::vec(3 * sourceVertices.size(), arma::fill::zeros);

      for(unsigned int i = 0; i < landmarks.size(); ++i) {

        const int index = landmarks.at(i).sourceIndex;
        const arma::vec& point = landmarks.at(i).targetPosition;

        for(int j = 0; j < 3; ++j) {

          linearizedLandmarkTarget(3 * index + j) = point(j);

        } // end for j

      } // end for i

    } // end linearize_landmark_target


    /*--------------------------------------------------------------------------*/

    EnergyData& energyData;
    EnergyDerivedData& energyDerivedData;
    EnergySettings& energySettings;

    /*--------------------------------------------------------------------------*/

  };
}

#endif
