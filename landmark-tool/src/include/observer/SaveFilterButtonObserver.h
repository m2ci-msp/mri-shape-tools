#ifndef __SAVE_FILTER_BUTTON_OBSERVER_H__
#define __SAVE_FILTER_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowSaveFilterDialog.h"
#include "singleton/GuiDescription.h"

class SaveFilterButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SaveFilterButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::Button* button;
      builder->get_widget("saveFilterButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &SaveFilterButtonObserver::clicked)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~SaveFilterButtonObserver() {
      this->clickHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowSaveFilterDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection clickHandler;
};

#endif
