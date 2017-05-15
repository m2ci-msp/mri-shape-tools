#ifndef __PLOT_POSITION_H__
#define __PLOT_POSITION_H__

#include <memory>

class PlotPosition {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<PlotPosition> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<PlotPosition>(new PlotPosition);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    void set_origin(const double& x, const double& y) {
      this->org_x = x;
      this->org_y = y;
    }

    /*----------------------------------------------------------------------*/

    void get_origin(double& x, double& y) const {
      x = this->org_x;
      y = this->org_y;
    }

  private:
    PlotPosition() {
      org_x = 0;
      org_y = 0;
    }

    /*----------------------------------------------------------------------*/

    double org_x;
    double org_y;

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<PlotPosition> instance;

};
#endif
