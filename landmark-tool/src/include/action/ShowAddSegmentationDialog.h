#ifndef __SHOW_ADD_SEGMENTATION_DIALOG_H__
#define __SHOW_ADD_SEGMENTATION_DIALOG_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Class that shows a "Open Scan" Dialog */
class ShowAddSegmentationDialog : public Action {

  public:
    /** Contructor that needs a Gtk builder object */
    ShowAddSegmentationDialog() {
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();
      // get the dialog ui element
      builder->get_widget("addSegmentationDialog", this->openDialog);
    }

    /*-----------------------------------------------------------------------*/

    virtual void execute() {
      openDialog->show();
    }

    /*-----------------------------------------------------------------------*/
  private:

    /** ui element for the dialog */
    Gtk::FileChooserDialog* openDialog;

};

#endif
