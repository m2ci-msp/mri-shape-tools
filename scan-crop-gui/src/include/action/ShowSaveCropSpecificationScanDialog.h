#ifndef __SHOW_SAVE_CROP_SPECIFICATION_DIALOG_H__
#define __SHOW_SAVE_CROP_SPECIFICATION_DIALOG_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Class that shows a "Open Image Stack" Dialog */
class ShowSaveCropSpecificationDialog: public Action {

public:
  /** Contructor that needs a Gtk builder object */
  ShowSaveCropSpecificationDialog() {
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();
    // get the dialog ui element
    builder->get_widget("saveCropSpecificationDialog", this->saveDialog);
  }

  /*-----------------------------------------------------------------------*/

  virtual void execute() {
    saveDialog->show();
  }

  /*-----------------------------------------------------------------------*/
private:
  /** ui element for the dialog */
  Gtk::FileChooserDialog* saveDialog;
};

#endif
