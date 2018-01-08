#include "image/Image.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image scan;
  scan.read().from(settings.inputScan);
  scan.values().scale(0, 255);

  const double stepSize = 0.125;

  scan.filter().diffusion(
                          settings.diffusionSettings.timeSteps,
                          stepSize,
                          settings.diffusionSettings.contrastLambda,
                          settings.diffusionSettings.integrationRho,
                          settings.diffusionSettings.presmoothSigma
                          );

  scan.write().to(settings.outputScan);

  return 0;

}
