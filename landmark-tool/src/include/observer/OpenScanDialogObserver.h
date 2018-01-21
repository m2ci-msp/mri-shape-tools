#ifndef __OPEN_SCAN_DIALOG_OBSERVER_H__
#define __OPEN_SCAN_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/OpenScan.h"
#include "action/ClearLandmarks.h"
#include "action/ShowErrorMessage.h"
#include "action/ShowFileInTitle.h"
#include "action/ShowMoreActions.h"
#include "action/HideSegmentationFrame.h"

/** Class that watches the open image stack dialog */
class OpenScanDialogObserver {
  public:
    OpenScanDialogObserver() { 

      // query GUI description for builder
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the ui element
      builder->get_widget("openScanDialog", this->dialog);

      // connect handler
      this->responseHandler = this->dialog->signal_response().connect(
          sigc::mem_fun(*this,
            &OpenScanDialogObserver::response_occured));
    }

    /*----------------------------------------------------------------------*/

    ~OpenScanDialogObserver() {
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
              OpenScan action(this->dialog->get_filename());
              action.execute();

              ShowFileInTitle show(this->dialog->get_filename());
              show.execute();

              ClearLandmarks clear;
              clear.execute();

              ShowMoreActions showActions;
              showActions.execute();

              HideSegmentationFrame hide;
              hide.execute();

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
