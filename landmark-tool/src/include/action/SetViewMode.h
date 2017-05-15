#ifndef __SET_VIEW_MODE_H__
#define __SET_VIEW_MODE_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/CoordinateSystem.h"
#include "singleton/Data.h"
#include "singleton/LandmarkPool.h"

/** action that sets the view mode in the singletons */
class SetViewMode : public Action {

  public:
    /*----------------------------------------------------------------------*/
    SetViewMode() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
	GuiDescription::get_instance()->get_builder();

      // get combox box for view mode
      builder->get_widget("viewMode", this->viewMode);
    }
    /*----------------------------------------------------------------------*/

    virtual void execute() {
      CoordinateSystem::get_instance()->set_view_mode(viewMode->get_active_row_number());
      Data::get_instance()->set_view_mode(viewMode->get_active_row_number());
      LandmarkPool::get_instance()->set_view_mode(viewMode->get_active_row_number());
    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::ComboBoxText* viewMode;
};
#endif
