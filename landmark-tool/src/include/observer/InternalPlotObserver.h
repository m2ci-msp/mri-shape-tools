#ifndef __INTERNAL_PLOT_OBSERVER_H__
#define __INTERNAL_PLOT_OBSERVER_H__

#include "singleton/InternalPlot.h"
#include "action/ConfigureScrollbars.h"
#include "action/DisplayPlot.h"
#include "action/RedrawArea.h"

class InternalPlotObserver {
  public:

    /*-----------------------------------------------------------------------*/

    InternalPlotObserver() {

      // connect handler to clicked signal
      this->plotChangedHandler =
        InternalPlot::get_instance()->internal_plot_changed.connect(
            sigc::mem_fun(*this, &InternalPlotObserver::plot_changed)
            );
    }


    /*-----------------------------------------------------------------------*/

    ~InternalPlotObserver() {
      this->plotChangedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void plot_changed() {

      ConfigureScrollbars configureScrollbars;
      configureScrollbars.execute();

      DisplayPlot displayPlot;
      displayPlot.execute();

      RedrawArea redraw;
      redraw.execute();

    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection plotChangedHandler;
};

#endif
