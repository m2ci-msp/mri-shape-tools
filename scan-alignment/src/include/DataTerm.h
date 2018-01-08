#ifndef __DATA_TERM_H__
#define __DATA_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "Energy.h"
#include "EnergyTerm.h"


#include "alignment/RigidTransformation.h"
#include "mesh/ProfileMesh.h"
#include "NormalizedCrossCorrelation.h"


/* class that represents the data term of the energy function */
class DataTerm : public EnergyTerm {

public:

  /*--------------------------------------------------------------------------*/

  DataTerm(Energy& energy) : energy(energy) {
  }

  /*--------------------------------------------------------------------------*/

  virtual void add_energy_and_gradient(
    double& energy,
    vnl_vector<double>& gradient) const {

    const double& tx = this->energy.data().transformation[0];
    const double& ty = this->energy.data().transformation[1];
    const double& tz = this->energy.data().transformation[2];

    const double& rx = this->energy.data().transformation[3];
    const double& ry = this->energy.data().transformation[4];
    const double& rz = this->energy.data().transformation[5];

    // add energy
    energy += compute_energy(tx, ty, tz, rx, ry, rz);

    add_translation_gradient(gradient);
    add_rotation_gradient(gradient);

  }

  /*--------------------------------------------------------------------------*/

  virtual double compute_current_energy() const {

    const double& tx = this->energy.data().transformation[0];
    const double& ty = this->energy.data().transformation[1];
    const double& tz = this->energy.data().transformation[2];

    const double& rx = this->energy.data().transformation[3];
    const double& ry = this->energy.data().transformation[4];
    const double& rz = this->energy.data().transformation[5];

    return compute_energy(tx, ty, tz, rx, ry, rz);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  double compute_energy(
    const double tx,
    const double ty,
    const double tz,
    const double rx,
    const double ry,
    const double rz
    ) const {


    RigidTransformation transformation;

    ProfileMesh transformed(
      this->energy.data().sourceMesh,
      this->energy.data().targetScan);

    transformation.set_origin(transformed.get_center());
    transformation.set_translation(tx, ty, tz);
    transformation.set_rotation(rx, ry, rz);

    transformation.apply(transformed);

    transformed\
      .set_length(this->energy.settings().profileLength)        \
      .set_offset(this->energy.settings().profileOffset)        \
      .compute_profiles();


    std::vector<arma::vec> targetProfiles = transformed.get_profiles();

    // return negative result -> we are minimizing
    return - NormalizedCrossCorrelation::compute(
      this->energy.data().sourceProfiles, targetProfiles
      );

  }

  /*--------------------------------------------------------------------------*/

  void add_translation_gradient(
    vnl_vector<double>& gradient
    ) const {

    const double& tx = this->energy.data().transformation[0];
    const double& ty = this->energy.data().transformation[1];
    const double& tz = this->energy.data().transformation[2];

    const double& rx = this->energy.data().transformation[3];
    const double& ry = this->energy.data().transformation[4];
    const double& rz = this->energy.data().transformation[5];


    const double& translationStep = this->energy.settings().translationStep;

    const double translationScale = 1. / ( 2. * translationStep);
    
    // tx derivative
    gradient[0] += translationScale * (
      compute_energy(tx + translationStep, ty, tz, rx, ry, rz) -
      compute_energy(tx - translationStep, ty, tz, rx, ry, rz)
      );
      
    // ty derivative
    gradient[1] += translationScale * (
      compute_energy(tx, ty + translationStep, tz, rx, ry, rz) -
      compute_energy(tx, ty - translationStep, tz, rx, ry, rz)
      );
 
    // tz derivative
    gradient[2] += translationScale * (
      compute_energy(tx, ty, tz + translationStep, rx, ry, rz) -
      compute_energy(tx, ty, tz - translationStep, rx, ry, rz)
      );

    return;
 
  }

  /*--------------------------------------------------------------------------*/

  void add_rotation_gradient(
    vnl_vector<double>& gradient
    ) const {

    const double& tx = this->energy.data().transformation[0];
    const double& ty = this->energy.data().transformation[1];
    const double& tz = this->energy.data().transformation[2];

    const double& rx = this->energy.data().transformation[3];
    const double& ry = this->energy.data().transformation[4];
    const double& rz = this->energy.data().transformation[5];


    const double& rotationStep = this->energy.settings().rotationStep;

    const double rotationScale = 1. / ( 2. * rotationStep);
    
    // rx derivative
    gradient[3] += rotationScale * (
      compute_energy(tx, ty, tz, rx + rotationStep, ry, rz) -
      compute_energy(tx, ty, tz, rx - rotationStep, ry, rz)
      );
      
    // ry derivative
    gradient[4] += rotationScale * (
      compute_energy(tx, ty, tz, rx, ry + rotationStep, rz) -
      compute_energy(tx, ty, tz, rx, ry - rotationStep, rz)
      );
 
    // rz derivative
    gradient[5] += rotationScale * (
      compute_energy(tx, ty, tz, rx, ry, rz + rotationStep) -
      compute_energy(tx, ty, tz, rx, ry, rz - rotationStep)
      );

    return;
 
  }

  /*--------------------------------------------------------------------------*/

  Energy& energy;

  /*--------------------------------------------------------------------------*/

};


#endif
