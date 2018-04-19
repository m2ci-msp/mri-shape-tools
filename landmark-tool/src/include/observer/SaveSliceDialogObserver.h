#ifndef __SAVE_SLICE_DIALOG_OBSERVER_H__
#define __SAVE_SLICE_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/SaveSlice.h"
#include "action/ShowErrorMessage.h"

/** Class that watches the save slice dialog */
class SaveSliceDialogObserver {
public:
  SaveSliceDialogObserver() { 

    // query GUI description for builder
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    // get the ui element
    builder->get_widget("saveSliceDialog", this->dialog);

    // connect handler
    this->responseHandler = this->dialog->signal_response().connect(
                                                                    sigc::mem_fun(*this,
                                                                                  &SaveSliceDialogObserver::response_occured));
  }

  /*----------------------------------------------------------------------*/

  ~SaveSliceDialogObserver() {
    this->responseHandler.disconnect();
  }
  /*----------------------------------------------------------------------*/

  void response_occured(int id) {
    // first hide the dialog
    this->dialog->hide();

    switch(id) {
    case 1:

      try {

        SaveSlice(
                  this->dialog->get_current_folder() + "/" +
                  this->dialog->get_current_name()
                  ).execute();

      }
      catch( const std::exception& e) {
        ShowErrorMessage message("Problem saving slice:", e);
        message.execute();
      }
      break;

    default:
      // do nothing
      break;
    }

  }

  /*----------------------------------------------------------------------*/

private:
  Gtk::FileChooserDialog* dialog;
  sigc::connection responseHandler;
};

#endif
