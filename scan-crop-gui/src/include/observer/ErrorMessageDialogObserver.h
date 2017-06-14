#ifndef __ERROR_MESSAGE_DIALOG_OBSERVER_H__
#define __ERROR_MESSAGE_DIALOG_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/HideErrorMessage.h"

class ErrorMessageDialogObserver {
  public:
    ErrorMessageDialogObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get widget to observe
      Gtk::MessageDialog* dialog;
      builder->get_widget("errorMessageDialog", dialog);

      // register handler
      this->responseHandler = dialog->signal_response().connect(
          sigc::mem_fun(*this, &ErrorMessageDialogObserver::response_received)
          );
    }

    /*----------------------------------------------------------------------*/

    ~ErrorMessageDialogObserver() {
      this->responseHandler.disconnect();
    }

    /*----------------------------------------------------------------------*/

    void response_received(int) {
      // always close the dialog
      HideErrorMessage action;
      action.execute();
    }

    /*----------------------------------------------------------------------*/

  private:
    sigc::connection responseHandler;

};
#endif
