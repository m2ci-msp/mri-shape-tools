#ifndef __SHOW_FILTER_WINDOW_OBSERVER_H__
#define __SHOW_FILTER_WINDOW_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowFilterWindow.h"
#include "singleton/GuiDescription.h"

class ShowFilterWindowObserver {
  public:

    /*-----------------------------------------------------------------------*/

    ShowFilterWindowObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get menu item to watch
      Gtk::MenuItem* item;
      builder->get_widget("showFilterItem", item);

      // connect handler to clicked signal
      this->activateHandler = item->signal_activate().connect(
                                                            sigc::mem_fun(*this, &ShowFilterWindowObserver::clicked)
                                                            );

    }


    /*-----------------------------------------------------------------------*/

    ~ShowFilterWindowObserver() {
      this->activateHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowFilterWindow action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:

    sigc::connection activateHandler;

};

#endif
