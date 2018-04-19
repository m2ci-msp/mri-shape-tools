#ifndef __OPEN_SCAN_BUTTON_OBSERVER_H__
#define __OPEN_SCAN_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "singleton/GuiDescription.h"
#include "action/ShowOpenScanDialog.h"

/** Class that watches the state of the openImageStackButton */
class OpenScanButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    OpenScanButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::ToolButton* button;
      builder->get_widget("openScanButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &OpenScanButtonObserver::clicked)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~OpenScanButtonObserver() {
      this->clickHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowOpenScanDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection clickHandler;
};

#endif
