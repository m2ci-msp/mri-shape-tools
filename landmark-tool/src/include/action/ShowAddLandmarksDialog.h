#ifndef __SHOW_ADD_LANDMARKS_DIALOG_H__
#define __SHOW_ADD_LANDMARKS_DIALOG_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Class that shows a "Add Landmarks" Dialog */
class ShowAddLandmarksDialog: public Action {

  public:
    /** Contructor that needs a Gtk builder object */
    ShowAddLandmarksDialog() {
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();
      // get the dialog ui element
      builder->get_widget("addLandmarksDialog", this->addDialog);
    }

    /*-----------------------------------------------------------------------*/

    virtual void execute() {
      addDialog->show();
    }

    /*-----------------------------------------------------------------------*/
  private:
    /** ui element for the dialog */
    Gtk::FileChooserDialog* addDialog;
};

#endif
