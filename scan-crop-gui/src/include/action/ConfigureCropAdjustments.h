#ifndef __CONFIGURE_CROP_ADJUSTMENTS_H__
#define __CONFIGURE_CROP_ADJUSTMENTS_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/Data.h"

/** action that configures the interface data of the
 * crop related adjustmends */
class ConfigureCropAdjustments : public Action {

  public:
    /*----------------------------------------------------------------------*/
    ConfigureCropAdjustments() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      this->minX = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("minXAdjustment"));
      this->maxX = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("maxXAdjustment"));

      this->minY = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("minYAdjustment"));
      this->maxY = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("maxYAdjustment"));

      this->minZ = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("minZAdjustment"));
      this->maxZ = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("maxZAdjustment"));


    }
    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // do not configure anything if no stack is available
      if( !Data::get_instance()->stack_present()) {
        return;
      }

      const int nx = Data::get_instance()->get_scan().data()->get_nx();
      const int ny = Data::get_instance()->get_scan().data()->get_ny();
      const int nz = Data::get_instance()->get_scan().data()->get_nz();

      this->minX->set_lower(0);
      this->minY->set_lower(0);
      this->minZ->set_lower(0);

      this->maxX->set_lower(0);
      this->maxY->set_lower(0);
      this->maxZ->set_lower(0);

      this->minX->set_upper(nx-1);
      this->minY->set_upper(ny-1);
      this->minZ->set_upper(nz-1);

      this->maxX->set_upper(nx-1);
      this->maxY->set_upper(ny-1);
      this->maxZ->set_upper(nz-1);

      this->maxX->set_value(nx-1);
      this->maxY->set_value(ny-1);
      this->maxZ->set_value(nz-1);

    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    Glib::RefPtr<Gtk::Adjustment> minX;
    Glib::RefPtr<Gtk::Adjustment> maxX;

    Glib::RefPtr<Gtk::Adjustment> minY;
    Glib::RefPtr<Gtk::Adjustment> maxY;

    Glib::RefPtr<Gtk::Adjustment> minZ;
    Glib::RefPtr<Gtk::Adjustment> maxZ;

};
#endif
