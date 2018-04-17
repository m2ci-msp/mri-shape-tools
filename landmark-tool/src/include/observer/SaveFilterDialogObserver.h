#ifndef __SAVE_FILTER_DIALOG_OBSERVER_H__
#define __SAVE_FILTER_DIALOG_OBSERVER_H__

#include <gtkmm/filechooserdialog.h>

#include "singleton/GuiDescription.h"
#include "action/SaveFilter.h"
#include "action/ShowErrorMessage.h"

class SaveFilterDialogObserver {
  public:
    SaveFilterDialogObserver() { 

      // query GUI description for builder
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the ui element
      builder->get_widget("saveFilterDialog", this->dialog);

      // connect handler
      this->responseHandler = this->dialog->signal_response().connect(
          sigc::mem_fun(*this,
            &SaveFilterDialogObserver::response_occured));
    }

    /*----------------------------------------------------------------------*/

    ~SaveFilterDialogObserver() {
      this->responseHandler.disconnect();
    }
    /*----------------------------------------------------------------------*/

    void response_occured(int id) {
      // first hide the dialog
      this->dialog->hide();

      switch(id) {
        case 1:
          try { 
            SaveFilter action(
              this->dialog->get_current_folder() + "/" +
              this->dialog->get_current_name());

            action.execute();
          }
          catch( const std::exception& e) {
            ShowErrorMessage message("Problem saving filter:", e);
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
