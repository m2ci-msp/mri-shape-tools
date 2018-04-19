#ifndef __VIEW_MODE_OBSERVER_H__
#define __VIEW_MODE_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/ConfigureSliceAdjustment.h"
#include "action/SetViewMode.h"
#include "action/SelectSlice.h"
#include "action/CleanDrawingArea.h"

/** Class that watches the state of the openImageStackButton */
class ViewModeObserver {
  public:

    /*-----------------------------------------------------------------------*/

    ViewModeObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::ComboBoxText* viewMode;
      builder->get_widget("viewMode", viewMode);

      // connect handler to clicked signal
      this->modeChangedHandler = viewMode->signal_changed().connect(
          sigc::mem_fun(*this, &ViewModeObserver::mode_changed)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~ViewModeObserver() {
      this->modeChangedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void mode_changed() {

      CleanDrawingArea clean;
      clean.execute();

      SetViewMode setViewMode;
      setViewMode.execute();

      ConfigureSliceAdjustment configure;
      configure.execute();

      SelectSlice select;
      select.execute();

    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection modeChangedHandler;
};

#endif
