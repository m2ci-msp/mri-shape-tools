#ifndef __SHOW_SAVE_LANDMARKS_DIALOG_H__
#define __SHOW_SAVE_LANDMARKS_DIALOG_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Class that shows a "Open Image Stack" Dialog */
class ShowSaveLandmarksDialog: public Action {

  public:
    /** Contructor that needs a Gtk builder object */
    ShowSaveLandmarksDialog() {
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();
      // get the dialog ui element
      builder->get_widget("saveLandmarksDialog", this->saveDialog);
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
