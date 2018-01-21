#ifndef __ADD_SEGMENTATION_DIALOG_OBSERVER_H__
#define __ADD_SEGMENTATION_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/OpenSegmentation.h"
#include "action/ShowErrorMessage.h"
#include "action/ShowSegmentationFrame.h"

/** Class that watches the open image stack dialog */
class AddSegmentationDialogObserver {
  public:
    AddSegmentationDialogObserver() { 

      // query GUI description for builder
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the ui element
      builder->get_widget("addSegmentationDialog", this->dialog);

      // connect handler
      this->responseHandler = this->dialog->signal_response().connect(
          sigc::mem_fun(*this,
            &AddSegmentationDialogObserver::response_occured));
    }

    /*----------------------------------------------------------------------*/

    ~AddSegmentationDialogObserver() {
      this->responseHandler.disconnect();
    }
    /*----------------------------------------------------------------------*/

    void response_occured(int id) {
      // first hide the dialog
      this->dialog->hide();

      switch(id) {
        case 1:
          {
            try{
              // try to build a image stack from the provided scan
              OpenSegmentation  action(this->dialog->get_filename());
              action.execute();

              ShowSegmentationFrame show;
              show.execute();

            }
            catch (const std::exception& e) {
              ShowErrorMessage message("Problem opening scan:", e);
              message.execute();
            }
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
