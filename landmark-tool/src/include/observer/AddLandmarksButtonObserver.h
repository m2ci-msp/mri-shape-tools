#ifndef __ADD_LANDMARKS_BUTTON_OBSERVER_H__
#define __ADD_LANDMARKS_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowAddLandmarksDialog.h"
#include "singleton/GuiDescription.h"

/** Class that watches the state of the openImageStackButton */
class AddLandmarksButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    AddLandmarksButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::ToolButton* button;
      builder->get_widget("addLandmarksButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &AddLandmarksButtonObserver::clicked)
          );

      // get menu item to watch
      Gtk::MenuItem* item;
      builder->get_widget("addLandmarksItem", item);

      // connect handler to activate signal
      this->activateHandler = item->signal_activate().connect(
                                                            sigc::mem_fun(*this, &AddLandmarksButtonObserver::clicked)
                                                            );

    }


    /*-----------------------------------------------------------------------*/

    ~AddLandmarksButtonObserver() {
      this->clickHandler.disconnect();
      this->activateHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowAddLandmarksDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection clickHandler;
    sigc::connection activateHandler;
};

#endif
