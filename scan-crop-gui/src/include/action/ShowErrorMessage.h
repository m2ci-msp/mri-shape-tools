#ifndef __SHOW_ERROR_MESSAGE_H__
#define __SHOW_ERROR_MESSAGE_H__

#include "singleton/GuiDescription.h"

/** Shows an error message dialog */
class ShowErrorMessage : public Action {

  public:
    ShowErrorMessage(
        const std::string& message,
        const std::exception& reason
        ) {

      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get error message dialog
      builder->get_widget("errorMessageDialog", this->dialog);

      this->message = message;
      this->reason = reason.what();
    }

    virtual void execute() {
      this->dialog->set_message(this->message);
      this->dialog->set_secondary_text(reason);
      this->dialog->show();
    }

  private:
    ShowErrorMessage();

    std::string message;
    std::string reason;
    Gtk::MessageDialog* dialog;
};
#endif
