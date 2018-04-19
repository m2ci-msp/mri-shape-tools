#ifndef __LUCAS_KANADE_ENERGY_SETTINGS_H__
#define __LUCAS_KANADE_ENERGY_SETTINGS_H__

namespace lucasKanade{

  class EnergySettings{

  public:

    EnergySettings() {

      this->epsilon = 0.00001;

    }

    double epsilon;

  };

}

#endif
