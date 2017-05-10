#ifndef __ENERGY_TERM_H__
#define __ENERGY_TERM_H__

class EnergyTerm{

public:

  virtual void add_energy_and_gradient(
    double& energy, vnl_vector<double>& gradient) const = 0;

  virtual ~EnergyTerm() {
  }

};

#endif
