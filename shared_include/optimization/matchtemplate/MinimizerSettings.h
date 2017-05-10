#ifndef __MATCH_TEMPLATE_MINIMIZER_SETTINGS_H__
#define __MATCH_TEMPLATE_MINIMIZER_SETTINGS_H__

namespace matchTemplate{

  class MinimizerSettings{

  public:

    int iterationAmount = 10;

    int convergenceFactor = 1e+7;
    double projectedGradientTolerance = 1e-5;
    int maxFunctionEvals = 1000;

  };

}

#endif
