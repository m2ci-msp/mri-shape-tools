#ifndef __SAVE_SEGMENTATION_BUTTON_OBSERVER_H__
#define __SAVE_SEGMENTATION_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowSaveSegmentationDialog.h"
#include "singleton/GuiDescription.h"

/** Class that watches the state of the openImageStackButton */
class SaveSegmentationButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SaveSegmentationButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::Button* button;
      builder->get_widget("saveSegmentationButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &SaveSegmentationButtonObserver::clicked)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~SaveSegmentationButtonObserver() {
      this->clickHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowSaveSegmentationDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection clickHandler;
};

#endif
