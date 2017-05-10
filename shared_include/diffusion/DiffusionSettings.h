#ifndef __DIFFUSION_SETTINGS_H__
#define __DIFFUSION_SETTINGS_H__

class DiffusionSettings{

public:

  int timeSteps = 1;
  double stepSize = 0.125;
  double contrastLambda = 1;
  double integrationRho = 1;
  double presmoothSigma = 1;

};

#endif
