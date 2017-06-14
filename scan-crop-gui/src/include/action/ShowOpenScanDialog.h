#ifndef __SHOW_OPEN_SCAN_DIALOG_H__
#define __SHOW_OPEN_SCAN_DIALOG_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Class that shows a "Open Scan" Dialog */
class ShowOpenScanDialog : public Action {

  public:
    /** Contructor that needs a Gtk builder object */
    ShowOpenScanDialog() {
      Glib::RefPtr<Gtk::Builder> builder = 
        GuiDescription::get_instance()->get_builder();
      // get the dialog ui element
      builder->get_widget("openScanDialog", this->openDialog);
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
