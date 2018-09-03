#ifndef __FIT_MODEL_MINIMIZER_SETTINGS_H__
#define __FIT_MODEL_MINIMIZER_SETTINGS_H__

namespace fitModel{

  class MinimizerSettings{

  public:

    int iterationAmount = 10;
    double priorSize = 1;

    // deactivate PCA behaviour if only one speaker component is available?
    bool noSpeakerPCA = false;

    // deactivate PCA behaviour if only one phoneme component is available?
    bool noPhonemePCA = false;

    int convergenceFactor = 1e+7;
    double projectedGradientTolerance = 1e-5;
    int maxFunctionEvals = 1000;

  };

}

#endif
