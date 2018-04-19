#ifndef __ADD_SEGMENTATION_BUTTON_OBSERVER_H__
#define __ADD_SEGMENTATION_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "singleton/GuiDescription.h"
#include "action/ShowAddSegmentationDialog.h"

/** Class that watches the state of the openImageStackButton */
class AddSegmentationButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    AddSegmentationButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get menu item to watch
      Gtk::MenuItem* item;
      builder->get_widget("loadSegmentationItem", item);

      // connect handler to clicked signal
      this->activateHandler = item->signal_activate().connect(
                                                            sigc::mem_fun(*this, &AddSegmentationButtonObserver::clicked)
                                                            );
    }


    /*-----------------------------------------------------------------------*/

    ~AddSegmentationButtonObserver() {
      this->activateHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowAddSegmentationDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:

    sigc::connection activateHandler;

};

#endif
