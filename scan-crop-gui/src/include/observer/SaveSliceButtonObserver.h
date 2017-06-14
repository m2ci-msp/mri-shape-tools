#ifndef __SAVE_SLICE_BUTTON_OBSERVER_H__
#define __SAVE_SLICE_BUTTON_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowSaveSliceDialog.h"
#include "singleton/GuiDescription.h"

/** Class that watches the state of the saveScreenButton */
class SaveSliceButtonObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SaveSliceButtonObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get button to watch
      Gtk::ToolButton* button;
      builder->get_widget("saveSliceButton", button);

      // connect handler to clicked signal
      this->clickHandler = button->signal_clicked().connect(
          sigc::mem_fun(*this, &SaveSliceButtonObserver::clicked)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~SaveSliceButtonObserver() {
      this->clickHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowSaveSliceDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection clickHandler;
};

#endif
