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

      // get button to watch
      Gtk::ToolButton* button;
      builder->get_widget("addSegmentationButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &AddSegmentationButtonObserver::clicked)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~AddSegmentationButtonObserver() {
      this->clickHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowAddSegmentationDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:

    sigc::connection clickHandler;

};

#endif
