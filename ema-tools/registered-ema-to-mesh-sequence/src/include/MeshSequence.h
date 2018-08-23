#ifndef __MESH_SEQUENCE_H__
#define __MESH_SEQUENCE_H__

#include <string>
#include <vector>
#include <utility>

#include <json/json.h>

#include "model/Model.h"
#include "mesh/Mesh.h"
#include "mesh/MeshCombiner.h"
#include "ema/Ema.h"
#include "ema/EmaCoordinateSystem.h"

#include "mesh-modify/ApplyModifications.h"
#include "registered-ema/RegisteredEma.h"

class MeshSequence{

private:

  Ema headMotion;

  RegisteredEma registeredEma;

  Model tongueModel;

  const Json::Value globalTransformation;

  double startTime;
  double endTime;


  // sampling rate in Hz
  double samplingRate;

  double timeStepSize;

  bool basic = true;

  Mesh coilMesh;

  bool showCoils = false;

public:

  MeshSequence(
               const RegisteredEma& registeredEma,
               const Model& tongueModel,
               const double& samplingRate
               ) :
    registeredEma(registeredEma),
    tongueModel(tongueModel),
    samplingRate(samplingRate) {

    this->basic = true;

    prepare_data_structures_basic();

    compute_time_step_size();


  }

  MeshSequence(
               const Ema& headMotion,
               const RegisteredEma& registeredEma,
               const Model& tongueModel,
               const Json::Value& globalTransformation,
               const double& startTime,
               const double& endTime,
               const double& samplingRate
               ) :

    headMotion(headMotion),
    registeredEma(registeredEma),
    tongueModel(tongueModel),
    globalTransformation(globalTransformation),
    startTime(startTime),
    endTime(endTime),
    samplingRate(samplingRate) {

    this->basic = false;

    prepare_data_structures_extended();

    compute_time_step_size();

  }

  void set_coil_mesh(const Mesh& mesh) {

    this->coilMesh = mesh;
    this->showCoils = true;

  }

  std::vector< std::pair<double, Mesh> > build() {

    // result contains the generated meshes with corresponding time stamps
    std::vector< std::pair<double, Mesh> > result;

    double currentTime = this->startTime;

    while( currentTime < this->endTime ) {

      Mesh currentMesh = reconstruct_mesh(currentTime);

      // add coil information if needed
      if(this->showCoils == true) {

        add_coils(currentMesh, currentTime);

      }

      // transform mesh if needed
      if( this->basic == false ) {

        apply_global_transformation(currentMesh);

        add_head_motion(currentMesh, currentTime);

      }

      result.push_back( std::make_pair(currentTime, currentMesh) );

      currentTime += timeStepSize;

    }

    return result;

  }

private:

  void add_coils(Mesh& mesh, const double& time) {

    std::vector<arma::vec> targetPoints = this->registeredEma.interpolate().target_points_at_time(time);

    for(const arma::vec& point: targetPoints) {

      // center coil mesh at current target point
      Mesh coil = this->coilMesh;

      arma::vec center = coil.get_center();

      for(arma::vec& vertex: coil.get_vertices()) {

        vertex = vertex - center + point;

      }

      // fuse with result mesh
      mesh = MeshCombiner::combine(mesh, coil);

    }

  }

  void compute_time_step_size() {

    this->timeStepSize = 1. / this->samplingRate;

  }

  // adjust boundaries and mirror data accordingly
  // we need a boundary size of 1 and von Neumann boundary conditions for the interpolation
  // extended setup -> generate meshes and transform them afterwards
  void prepare_data_structures_extended() {

    std::vector<std::string> coils({"referenceLeft", "referenceRight", "referenceFront"});

    for(const std::string& coil: coils) {

      this->headMotion.coil(coil).boundary().change_size(1);

      this->headMotion.coil(coil).mirror().positions();

    }

    this->registeredEma.boundary().change_size(1);
    this->registeredEma.mirror().speaker_weights();
    this->registeredEma.mirror().phoneme_weights();
    this->registeredEma.mirror().target_points();

    // adjust end time if needed, the ema files might be slightly shorter than the wanted end time
    // TODO: raise exception if difference is above 0.5 seconds
    this->endTime = ( this->registeredEma.info().time_stamps().back() < this->endTime) ?
      this->registeredEma.info().time_stamps().back() :
      this->endTime;

    this->endTime = ( this->headMotion.info().time_stamps().back() < this->endTime) ?
      this->headMotion.info().time_stamps().back() :
      this->endTime;

  }

  // data structures for generating only the meshes
  void prepare_data_structures_basic() {

    this->registeredEma.boundary().change_size(1);
    this->registeredEma.mirror().speaker_weights();
    this->registeredEma.mirror().phoneme_weights();

    this->endTime = this->registeredEma.info().time_stamps().back();
    this->startTime = 0;

  }

  Mesh reconstruct_mesh(const double& timeStamp) {

    const arma::vec speakerWeights = this->registeredEma.interpolate().speaker_weight_at_time(timeStamp);
    const arma::vec phonemeWeights = this->registeredEma.interpolate().phoneme_weight_at_time(timeStamp);

    const Mesh reconstructed = this->tongueModel.reconstruct_mesh().for_weights(speakerWeights, phonemeWeights);

    return reconstructed;

  }

  void apply_global_transformation(Mesh& mesh) {

    meshModify::ApplyModifications(mesh).apply(this->globalTransformation);

  }

  void add_head_motion(Mesh& mesh, const double& timeStamp) {

    const arma::vec left = this->headMotion.coil("referenceLeft").interpolate().position_at_time(timeStamp);
    const arma::vec right = this->headMotion.coil("referenceRight").interpolate().position_at_time(timeStamp);
    const arma::vec front = this->headMotion.coil("referenceFront").interpolate().position_at_time(timeStamp);

    EmaCoordinateSystem system;
    system.build_from(left, right, front);

    for(arma::vec& vertex: mesh.get_vertices() ) {

      vertex = system.map_back(vertex);

    }

  }

};
#endif
