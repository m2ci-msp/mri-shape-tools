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
      this->activateHandler = button->signal_clicked().connect(
                                                              sigc::mem_fun(*this, &OpenScanButtonObserver::clicked)
                                                              );

      // get menu item to observe
      Gtk::MenuItem* item;
      builder->get_widget("openScanItem", item);

      // connect handler to activate signal
      this->activateHandler = item->signal_activate().connect(
          sigc::mem_fun(*this, &OpenScanButtonObserver::clicked)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~OpenScanButtonObserver() {

      this->clickHandler.disconnect();

      this->activateHandler.disconnect();

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
    sigc::connection activateHandler;

};

#endif
