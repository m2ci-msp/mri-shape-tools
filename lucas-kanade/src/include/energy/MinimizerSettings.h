#ifndef __LUCAS_KANADE_MINIMIZER_SETTINGS_H__
#define __LUCAS_KANADE_MINIMIZER_SETTINGS_H__

namespace lucasKanade{
  class MinimizerSettings{

  public:

    int iterationAmount = 10;

    int convergenceFactor = 1000;
    double projectedGradientTolerance = 0.001;
    int maxFunctionEvals = 50;

  };

}

#endif
