#ifndef __HIDE_ERROR_MESSAGE_H__
#define __HIDE_ERROR_MESSAGE_H__

/** Hides the error dialog */
class HideErrorMessage : public Action {
  public:
    HideErrorMessage() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();

      // get error message dialog
      builder->get_widget("errorMessageDialog", this->dialog);

    }

    virtual void execute() {
      this->dialog->hide();
    }

  private:

    Gtk::MessageDialog* dialog;
};
#endif
