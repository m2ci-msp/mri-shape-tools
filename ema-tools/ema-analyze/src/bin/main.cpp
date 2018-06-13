#include "ema/Ema.h"
#include "ExportRMS.h"
#include "ExportSpeed.h"
#include "ExportPosition.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  ema.read().from(settings.input);

  const EmaInfo& info = ema.info();

  if( settings.exportInfo == "RMS" ) {

    const EmaCoilAccess& coil = ema.coil(settings.coil).access();
    ExportRMS(coil, info).to(settings.output);

  }
  else if( settings.exportInfo == "speed" ) {

    ema.coil(settings.coil).boundary().change_size(1);
    ema.coil(settings.coil).mirror().positions();
    const EmaCoilCalculus& coil = ema.coil(settings.coil).calculus();
    ExportSpeed(coil, info).to(settings.output);

  }
  else if( settings.exportInfo == "position" ) {

    const EmaCoilAccess& coil = ema.coil(settings.coil).access();
    ExportPosition(coil, info).to(settings.output);

  }

  return 0;

}
