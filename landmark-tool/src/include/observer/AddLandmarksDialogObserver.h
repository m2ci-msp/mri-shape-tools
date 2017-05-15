#ifndef __ADD_LANDMARKS_DIALOG_OBSERVER_H__
#define __ADD_LANDMARKS_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/ReadLandmarks.h"
#include "action/RefreshLandmarksView.h"
#include "action/ShowErrorMessage.h"
#include "observer/Observatory.h"
#include "observer/LandmarksObserver.h"
#include "observer/LandmarkTreeViewObserver.h"

/** Class that watches the open image stack dialog */
class AddLandmarksDialogObserver {
  public:
    AddLandmarksDialogObserver() { 

      // query GUI description for builder
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the ui element
      builder->get_widget("addLandmarksDialog", this->dialog);

      // connect handler
      this->responseHandler = this->dialog->signal_response().connect(
          sigc::mem_fun(*this,
            &AddLandmarksDialogObserver::response_occured));
    }

    /*----------------------------------------------------------------------*/

    ~AddLandmarksDialogObserver() {
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

              Observatory<LandmarksObserver>::destroy();
              Observatory<LandmarkTreeViewObserver>::destroy();
              ReadLandmarks read(this->dialog->get_filename());

              read.execute();

              RefreshLandmarksView refresh;
              refresh.execute();

              CreateInternalPlot createPlot;
              createPlot.execute();

            }
            catch( const std::exception& e ) {
              ShowErrorMessage message("Problem opening landmarks file:", e);
              message.execute();
            }

            Observatory<LandmarksObserver>::create();
            Observatory<LandmarkTreeViewObserver>::create();
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
