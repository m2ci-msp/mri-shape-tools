#ifndef __MINIMIZER_SETTINGS_H__
#define __MINIMIZER_SETTINGS_H__

class MinimizerSettings{

public:

  int iterationAmount = 10;
  double translationLimit = 20;
  double rotationLimit = 20;

  int convergenceFactor = 1e+7;
  double projectedGradientTolerance = 1e-5;
  int maxFunctionEvals = 1000;

};

#endif
