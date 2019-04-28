#include "registered-ema/RegisteredEma.h"

#include "ExportGradientNorm.h"
#include "ExportWeightsNorm.h"
#include "ExportVariationsNorm.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  RegisteredEma registeredEma;

  registeredEma.read().from(settings.input);

  if( settings.exportInfo == "gradientNorm") {

    registeredEma.boundary().change_size(1);
    registeredEma.mirror().speaker_weights();
    registeredEma.mirror().phoneme_weights();
    ExportGradientNorm(registeredEma).to(settings.output);

  }
  else if( settings.exportInfo == "weightsNorm") {

    ExportWeightsNorm(registeredEma).to(settings.output);

  }
  else if( settings.exportInfo == "variationsNorm") {

    ExportVariationsNorm(registeredEma).to(settings.output);

  }

  return 0;

}
