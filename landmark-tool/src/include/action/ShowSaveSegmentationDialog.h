#ifndef __SHOW_SAVE_SEGMENTATION_DIALOG_H__
#define __SHOW_SAVE_SEGMENTATION_DIALOG_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Class that shows a "Open Image Stack" Dialog */
class ShowSaveSegmentationDialog: public Action {

  public:
    /** Contructor that needs a Gtk builder object */
    ShowSaveSegmentationDialog() {
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();
      // get the dialog ui element
      builder->get_widget("saveSegmentationDialog", this->saveDialog);
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
