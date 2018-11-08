#ifndef __EMA_GENERATOR_H__
#define __EMA_GENERATOR_H__

#include <string>
#include <vector>
#include <utility>

#include <json/json.h>

#include "mesh/Mesh.h"
#include "ema/Ema.h"
#include "ema/EmaCoordinateSystem.h"

#include "mesh-modify/ApplyModifications.h"
#include "registered-ema/RegisteredEma.h"

class EmaGenerator{

private:

  Ema headMotion;

  RegisteredEma registeredEma;

  const Json::Value globalTransformation;
  Json::Value customTransformation;

  bool customTransformationPresent = false;

  double startTime;
  double endTime;

  // sampling rate in Hz
  double samplingRate;

  std::vector<std::string> coilIds;

  double timeStepSize;

  bool basic = true;

  std::vector<double> timeStamps;

public:

  EmaGenerator(
               const RegisteredEma& registeredEma,
               const double& samplingRate,
               std::vector<std::string>& coilIds
               ) :
    registeredEma(registeredEma),
    samplingRate(samplingRate),
    coilIds(coilIds) {

    this->basic = true;

    prepare_data_structures_basic();

    compute_time_step_size();

    compute_time_stamps();


  }

  EmaGenerator(
               const Ema& headMotion,
               const RegisteredEma& registeredEma,
               const Json::Value& globalTransformation,
               const double& startTime,
               const double& endTime,
               const double& samplingRate,
               std::vector<std::string>& coilIds
               ) :

    headMotion(headMotion),
    registeredEma(registeredEma),
    globalTransformation(globalTransformation),
    startTime(startTime),
    endTime(endTime),
    samplingRate(samplingRate),
    coilIds(coilIds) {

    this->basic = false;

    prepare_data_structures_extended();

    compute_time_step_size();

    compute_time_stamps();

  }

  void set_start_time(const double& time) {

    this->startTime = time;

  }

  void set_end_time(const double& time) {

    this->endTime = time;

  }

  void set_custom_transformation(Json::Value& customTransformation) {

    this->customTransformation = customTransformation;
    this->customTransformationPresent = true;

  }

  std::vector< std::vector<arma::vec> > build_target_points() {

    std::vector< std::vector<arma::vec> > targetPoints;

    for( const double& currentTime : this->timeStamps) {

      std::vector<arma::vec> currentPoints =
        this->registeredEma.interpolate().target_points_at_time(currentTime);

      // build mesh from coil positions
      Mesh currentMesh;
      currentMesh.set_vertices(currentPoints);

      Mesh copy = currentMesh;

      // transform mesh if needed
      if( this->basic == false ) {

        apply_global_transformation(currentMesh);

        add_head_motion(currentMesh, currentTime);

      }

      // estimate translation between mesh with custom transformation and transformed mesh
      // and apply this transformation to custom mesh if needed
      if( this->customTransformationPresent == true) {

        meshModify::ApplyModifications(copy).apply(this->customTransformation);

        arma::vec customCenter = copy.get_center();
        arma::vec newCenter = currentMesh.get_center();

        currentMesh = copy;

        for(arma::vec& vertex: currentMesh.get_vertices()) {

          vertex += newCenter - customCenter;

        }

      }

      // extract coil positions from transformed mesh
      currentPoints = currentMesh.get_vertices();

      // add points to data structure
      for(size_t i = 0; i < currentPoints.size(); ++i) {

        targetPoints[i].push_back(currentPoints.at(i));

      }

    } // end for timeStamps

    return targetPoints;

  }

  Ema build() {

    EmaData emaData;

    emaData.emaInfoData.timeStamps = this->timeStamps;
    emaData.emaInfoData.samplingFrequency = this->samplingRate;
    emaData.emaInfoData.coilIds = this->coilIds;

    std::vector< std::vector<arma::vec> > targetPoints = build_target_points();

    if( targetPoints.size() != this->coilIds.size()) {

      throw std::logic_error("mismatch between present coil ids and available target points.");

    }

    std::map<std::string, EmaCoil> emaCoils;

    for(size_t i = 0; i < this->coilIds.size(); ++i) {

      EmaCoilData data(emaData.emaInfoData);
      data.positions = targetPoints.at(i);
      data.id = this->coilIds.at(i);
      emaCoils.emplace(this->coilIds.at(i), EmaCoil(data));

    }

    emaData.emaCoils = emaCoils;

    Ema result;

    result.build().from(emaData);

    return result;

  }

private:

  void compute_time_stamps() {

    double currentTime = this->startTime;

    while( currentTime < this->endTime ) {

      this->timeStamps.push_back(currentTime);
      currentTime += timeStepSize;

    }

  }

  void compute_time_step_size() {

    this->timeStepSize = 1. / this->samplingRate;

  }

  // adjust boundaries and mirror data accordingly
  // we need a boundary size of 1 and von Neumann boundary conditions for the interpolation
  // extended setup -> generate ema and transform it afterwards
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

  // data structures for generating only the ema data
  void prepare_data_structures_basic() {

    this->registeredEma.boundary().change_size(1);
    this->registeredEma.mirror().speaker_weights();
    this->registeredEma.mirror().phoneme_weights();
    this->registeredEma.mirror().target_points();

    this->endTime = this->registeredEma.info().time_stamps().back();
    this->startTime = 0;

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
