#ifndef __MATCH_TEMPLATE_ENERGY_DERIVED_DATA_UPDATE_H__
#define __MATCH_TEMPLATE_ENERGY_DERIVED_DATA_UPDATE_H__

#include "mesh/NormalEstimation.h"

#include "optimization/matchtemplate/EnergyData.h"
#include "optimization/matchtemplate/EnergyDerivedData.h"
#include "optimization/matchtemplate/EnergySettings.h"
#include "optimization/matchtemplate/Transformation.h"
#include "optimization/matchtemplate/TransformationEnum.h"

namespace matchTemplate{

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

    /* computes smoothness term and landmark term weights for given iteration */
    void for_iteration(const int& i) {

      this->energyDerivedData.weights["landmarkTerm"] =
        this->energySettings.weights["landmarkTerm"] -
        i * this->energySettings.changeFactors["landmarkTerm"];

      this->energyDerivedData.weights["smoothnessTerm"] =
        this->energySettings.weights["smoothnessTerm"] -
        i * this->energySettings.changeFactors["smoothnessTerm"];

    }

    /*--------------------------------------------------------------------------*/

    /* updates normals of the source mesh that depend on the current transformations */
    void source_normals() {

      NormalEstimation estimator(this->energyDerivedData.source);
      this->energyDerivedData.source.set_vertex_normals(estimator.compute());

    }

    /*--------------------------------------------------------------------------*/

    /* updates the vertices of the source mesh
     * that depend on the chosen transformations
     */
    void for_transformations() {

      construct_transformations();

      transform_source();

    }

    /*--------------------------------------------------------------------------*/

    /* updates data structures that depend on the neighbor correspondences */
    void for_neighbors() {

      compute_neighbor_positions();

      // update data term weight
      const double weight = this->energySettings.weights.at("dataTerm");

      this->energyDerivedData.weights["dataTerm"] = weight /
        ( ( this->energyDerivedData.sourceIndices.size() == 0)?
          1. : this->energyDerivedData.sourceIndices.size() );

    }

    /*--------------------------------------------------------------------------*/

    /* update data structures that depend on the landmarks */
    void for_landmarks() {

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

    void compute_neighbor_positions() {

      // setup helper variables for accessing the necessary data
      const std::vector<arma::vec>& sourceVertices =
        this->energyDerivedData.source.get_vertices();

      const std::vector<arma::vec>& targetVertices =
        this->energyData.target.get_vertices();

      const std::vector<int>& sourceIndices =
        this->energyDerivedData.sourceIndices;

      const std::vector<int>& targetIndices =
        this->energyDerivedData.targetIndices;

      this->energyDerivedData.neighbors =
        std::vector<arma::vec> ( this->energyData.vertexAmount, arma::zeros(3) );
      this->energyDerivedData.neighborIndicator =
        std::vector<int> ( this->energyData.vertexAmount, 0 );

      // setup end

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

        this->energyDerivedData.neighbors[sourceIndex]         = targetPoint;
        this->energyDerivedData.neighborIndicator[sourceIndex] = 1;

      } // end for i

    }

    /*--------------------------------------------------------------------------*/

    /* transforms original template according to the current transformation
     * parameters
     */
    void transform_source() {


      Mesh& source = this->energyData.source;
      Mesh& transformedSource = this->energyDerivedData.source;
      const int vertexAmount = source.get_vertices().size();

      for(int i = 0; i < vertexAmount; ++i) {

        const arma::vec& vertex = source.get_vertices().at(i);
        arma::vec& transformedVertex = transformedSource.get_vertices().at(i);
        const Transformation& transformation = this->energyDerivedData.transformations.at(i);

        transformedVertex = transformation.apply_matrix(vertex);

      }

    }

    /*--------------------------------------------------------------------------*/

    /* constructs rotation and translation objects from the serialized
     * transformation data in EnergyData
     */
    void construct_transformations() {

      // clear transformations
      this->energyDerivedData.transformations.clear();

      const int vertexAmount = this->energyData.source.get_vertices().size();
      const int& transformationAmount = this->energyData.transformationAmount;

      for( int i = 0; i < vertexAmount * transformationAmount; i += transformationAmount) {

        // first read translation
        const double tx = this->energyData.transformations[i + TX];
        const double ty = this->energyData.transformations[i + TY];
        const double tz = this->energyData.transformations[i + TZ];

        // read rotation angle
        const double theta = this->energyData.transformations[i + THETA];

        // read rotation axis
        const double ax = this->energyData.transformations[i + AX];
        const double ay = this->energyData.transformations[i + AY];
        const double az = this->energyData.transformations[i + AZ];

        // build transformation objects
        Translation translation(tx, ty, tz);
        Rotation rotation(ax, ay, az, theta);

        this->energyDerivedData.transformations.push_back(Transformation(rotation, translation));

      }

    }

    /*--------------------------------------------------------------------------*/

    EnergyData& energyData;
    EnergyDerivedData& energyDerivedData;
    EnergySettings& energySettings;

    /*--------------------------------------------------------------------------*/

  };
}

#endif
