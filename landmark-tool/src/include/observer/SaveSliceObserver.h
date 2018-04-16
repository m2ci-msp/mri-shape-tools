#ifndef __SAVE_SLICE_OBSERVER_H__
#define __SAVE_SLICE_OBSERVER_H__

#include <gtkmm/toolbutton.h>

#include "action/ShowSaveSliceDialog.h"
#include "singleton/GuiDescription.h"

class SaveSliceObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SaveSliceObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get menu item to watch
      Gtk::MenuItem* item;
      builder->get_widget("saveSliceItem", item);

      // connect handler to clicked signal
      this->activateHandler = item->signal_activate().connect(
                                                            sigc::mem_fun(*this, &SaveSliceObserver::clicked)
                                                            );

    }


    /*-----------------------------------------------------------------------*/

    ~SaveSliceObserver() {
      this->activateHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void clicked() {
      // open the dialog
      ShowSaveSliceDialog action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:

    sigc::connection activateHandler;

};

#endif
