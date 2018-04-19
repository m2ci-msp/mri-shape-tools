#ifndef __SAVE_SEGMENTATION_DIALOG_OBSERVER_H__
#define __SAVE_SEGMENTATION_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/SaveSegmentation.h"
#include "action/ShowErrorMessage.h"

/** Class that watches the open image stack dialog */
class SaveSegmentationDialogObserver {
  public:
    SaveSegmentationDialogObserver() { 

      // query GUI description for builder
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the ui element
      builder->get_widget("saveSegmentationDialog", this->dialog);

      // connect handler
      this->responseHandler = this->dialog->signal_response().connect(
          sigc::mem_fun(*this,
            &SaveSegmentationDialogObserver::response_occured));
    }

    /*----------------------------------------------------------------------*/

    ~SaveSegmentationDialogObserver() {
      this->responseHandler.disconnect();
    }
    /*----------------------------------------------------------------------*/

    void response_occured(int id) {
      // first hide the dialog
      this->dialog->hide();

      switch(id) {
        case 1:
          try { 
            SaveSegmentation action(
              this->dialog->get_current_folder() + "/" +
              this->dialog->get_current_name());

            action.execute();
          }
          catch( const std::exception& e) {
            ShowErrorMessage message("Problem saving segmentation:", e);
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
