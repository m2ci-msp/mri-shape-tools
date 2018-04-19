#ifndef __SHOW_SEGMENTATION_WINDOW_OBSERVER_H__
#define __SHOW_SEGMENTATION_WINDOW_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowSegmentationWindow.h"
#include "singleton/GuiDescription.h"

class ShowSegmentationWindowObserver {
  public:

    /*-----------------------------------------------------------------------*/

    ShowSegmentationWindowObserver() {

      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get menu item to watch
      Gtk::MenuItem* item;
      builder->get_widget("showSegmentationItem", item);

      // connect handler to clicked signal
      this->activateHandler = item->signal_activate().connect(
                                                            sigc::mem_fun(*this, &ShowSegmentationWindowObserver::clicked)
                                                            );

    }


    /*-----------------------------------------------------------------------*/

    ~ShowSegmentationWindowObserver() {
      this->activateHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowSegmentationWindow action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:

    sigc::connection activateHandler;

};

#endif
