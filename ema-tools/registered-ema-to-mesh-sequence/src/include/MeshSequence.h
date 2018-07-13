#ifndef __MESH_SEQUENCE_H__
#define __MESH_SEQUENCE_H__

#include <string>
#include <vector>
#include <utility>

#include <json/json.h>

#include "model/Model.h"
#include "mesh/Mesh.h"
#include "ema/Ema.h"
#include "ema/EmaCoordinateSystem.h"

#include "mesh-modify/ApplyModifications.h"
#include "RegisteredEma.h"

class MeshSequence{

private:

  Ema headMotion;

  RegisteredEma registeredEma;

  Model tongueModel;

  std::string globalTransformation;

  double startTime;
  double endTime;


  // sampling rate in Hz
  double samplingRate;

  double timeStepSize;

  double scaleFactor;

public:

  MeshSequence(
               const Ema& headMotion,
               const RegisteredEma& registeredEma,
               const Model& tongueModel,
               const std::string& globalTransformation,
               const double& startTime,
               const double& endTime,
               const double& samplingRate,
               const double& scaleFactor
               ) :

    headMotion(headMotion),
    registeredEma(registeredEma),
    tongueModel(tongueModel),
    globalTransformation(globalTransformation),
    startTime(startTime),
    endTime(endTime),
    samplingRate(samplingRate),
    scaleFactor(scaleFactor){

    prepare_data_structures();

    compute_time_step_size();

  }

  std::vector< std::pair<double, Mesh> > build() {

    // result contains the generated meshes with corresponding time stamps
    std::vector< std::pair<double, Mesh> > result;

    double currentTime = this->startTime;

    while( currentTime < this->endTime ) {

      Mesh currentMesh = reconstruct_mesh(currentTime);

      apply_global_transformation(currentMesh);

      add_head_motion(currentMesh, currentTime);

      rescale_mesh(currentMesh);

      result.push_back( std::make_pair(currentTime, currentMesh) );

      currentTime += timeStepSize;

    }

    return result;

  }

private:

  void compute_time_step_size() {

    this->timeStepSize = 1. / this->samplingRate;

  }

  // adjust boundaries and mirror data accordingly
  // we need a boundary size of 1 and von Neumann boundary conditions for the interpolation
  void prepare_data_structures() {

    std::vector<std::string> coils({"left", "right", "front"});

    for(const std::string& coil: coils) {

      this->headMotion.coil(coil).boundary().change_size(1);

      this->headMotion.coil(coil).mirror().positions();

    }

    this->registeredEma.boundary().change_size(1);
    this->registeredEma.mirror().speaker_weights();
    this->registeredEma.mirror().phoneme_weights();

    // adjust end time if needed, the ema files might be slightly shorter than the wanted end time
    // TODO: raise exception if difference is above 0.5 seconds
    this->endTime = ( this->registeredEma.info().time_stamps().back() < this->endTime) ?
      this->registeredEma.info().time_stamps().back() :
      this->endTime;

    this->endTime = ( this->headMotion.info().time_stamps().back() < this->endTime) ?
      this->headMotion.info().time_stamps().back() :
      this->endTime;

  }

  Mesh reconstruct_mesh(const double& timeStamp) {

    const arma::vec speakerWeights = this->registeredEma.interpolate().speaker_weight_at_time(timeStamp);
    const arma::vec phonemeWeights = this->registeredEma.interpolate().phoneme_weight_at_time(timeStamp);

    const Mesh reconstructed = this->tongueModel.reconstruct_mesh().for_weights(speakerWeights, phonemeWeights);

    return reconstructed;

  }

  void apply_global_transformation(Mesh& mesh) {

    ApplyModifications(mesh).apply(this->globalTransformation);

  }

  void add_head_motion(Mesh& mesh, const double& timeStamp) {

    const arma::vec left = this->headMotion.coil("left").interpolate().position_at_time(timeStamp);
    const arma::vec right = this->headMotion.coil("right").interpolate().position_at_time(timeStamp);
    const arma::vec front = this->headMotion.coil("front").interpolate().position_at_time(timeStamp);

    EmaCoordinateSystem system;
    system.build_from(left, right, front);

    for(arma::vec& vertex: mesh.get_vertices() ) {

      vertex = system.map_back(vertex);

    }

  }

  void rescale_mesh(Mesh& mesh) {

    for(arma::vec& vertex: mesh.get_vertices() ) {

      vertex *= this->scaleFactor;

    }

  }

};
#endif
