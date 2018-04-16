#ifndef __SAVE_LANDMARKS_BUTTON_OBSERVER_H__
#define __SAVE_LANDMARKS_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowSaveLandmarksDialog.h"
#include "singleton/GuiDescription.h"

/** Class that watches the state of the openImageStackButton */
class SaveLandmarksButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SaveLandmarksButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::ToolButton* button;
      builder->get_widget("saveLandmarksButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &SaveLandmarksButtonObserver::clicked)
          );

      // get menu item to watch
      Gtk::MenuItem* item;
      builder->get_widget("saveLandmarksItem", item);

      // connect handler to clicked signal
      this->activateHandler = item->signal_activate().connect(
                                                            sigc::mem_fun(*this, &SaveLandmarksButtonObserver::clicked)
                                                            );

    }


    /*-----------------------------------------------------------------------*/

    ~SaveLandmarksButtonObserver() {
      this->clickHandler.disconnect();
      this->activateHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowSaveLandmarksDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:

    sigc::connection clickHandler;
    sigc::connection activateHandler;

};

#endif
