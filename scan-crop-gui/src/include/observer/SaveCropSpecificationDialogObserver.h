#ifndef __SAVE_CROP_SPECIFICATION_DIALOG_OBSERVER_H__
#define __SAVE_CROP_SPECIFICATION_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/SaveCropSpecification.h"
#include "action/ShowErrorMessage.h"

/** Class that watches the open image stack dialog */
class SaveCropSpecificationDialogObserver {
  public:
    SaveCropSpecificationDialogObserver() {

      // query GUI description for builder
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the ui element
      builder->get_widget("saveCropSpecificationDialog", this->dialog);

      // connect handler
      this->responseHandler = this->dialog->signal_response().connect(
          sigc::mem_fun(*this,
            &SaveCropSpecificationDialogObserver::response_occured));
    }

    /*----------------------------------------------------------------------*/

    ~SaveCropSpecificationDialogObserver() {
      this->responseHandler.disconnect();
    }
    /*----------------------------------------------------------------------*/

    void response_occured(int id) {
      // first hide the dialog
      this->dialog->hide();

      switch(id) {
        case 1:
          try {
            SaveCropSpecification action(
              this->dialog->get_current_folder() + "/" +
              this->dialog->get_current_name());

            action.execute();
          }
          catch( const std::exception& e) {
            ShowErrorMessage message("Problem saving scan:", e);
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
