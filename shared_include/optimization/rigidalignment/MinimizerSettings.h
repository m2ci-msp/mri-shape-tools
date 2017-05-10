#ifndef __RIGID_ALIGNMENT_MINIMIZER_SETTINGS_H__
#define __RIGID_ALIGNMENT_MINIMIZER_SETTINGS_H__

namespace rigidAlignment{
  class MinimizerSettings{

  public:

    int iterationAmount = 10;
    bool noTranslation = false;
    bool noRotation = false;

    int convergenceFactor = 1000;
    double projectedGradientTolerance = 0.001;
    int maxFunctionEvals = 50;

  };

}

#endif
