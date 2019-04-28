#ifndef __RIGID_ALIGNMENT_MINIMIZER_SETTINGS_H__
#define __RIGID_ALIGNMENT_MINIMIZER_SETTINGS_H__

namespace rigidAlignment{
  class MinimizerSettings{

  public:

    int iterationAmount = 10;
    bool noTranslation = false;
    bool noRotation = false;
    bool noScaling = false;

    int convergenceFactor = 1e+7;
    double projectedGradientTolerance = 1e-5;
    int maxFunctionEvals = 1000;

  };

}

#endif
