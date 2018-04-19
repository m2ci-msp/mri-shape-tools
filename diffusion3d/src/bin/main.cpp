#include "image/Image.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image scan;
  scan.read().from(settings.inputScan);
  scan.values().scale(0, 255);

  scan.filter().diffusion(
                          settings.timeSteps,
                          settings.stepSize,
                          settings.contrastLambda,
                          settings.integrationRho,
                          settings.presmoothSigma
                          );

  scan.write().to(settings.outputScan);

  return 0;

}
