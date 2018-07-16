#ifndef __CHAN_VESE_SETTINGS_H__
#define __CHAN_VESE_SETTINGS_H__

class ChanVeseSettings{

public:

  // time steps to take for the evolution
  int evolutionSteps = 1;

  // epsilon value for calculating the regularized variants of the absolute value, Heaviside, and dirac delta
  // functions
  double epsilon = 0.0001;

  // iteration amount for solving the linear system of equations
  int iterationAmount = 100;

  // time step size between the evolution steps
  double timeStepSize = 0.1;

  // penalizer weight for the curve length
  double mu = 1;

  // penalizer weights for the variances in both regions
  double lambdaRegionOne = 1;
  double lambdaRegionTwo = 1;

  // penalizer weight for the area of region one
  double eta = 0;

  // relaxation parameter in the successive overrelaxation
  double omega = 1.97;

};

#endif
