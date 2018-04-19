#ifndef __CROP_OBSERVER_H__
#define __CROP_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/CreateInternalPlot.h"

class CropObserver {
  public:

    /*-----------------------------------------------------------------------*/

    CropObserver() {

      auto builder = GuiDescription::get_instance()->get_builder();

      // observe all adjustments related to cropping
      Glib::RefPtr<Gtk::Adjustment> adjustment;

      std::vector<std::string> ids({
        "minXAdjustment", 
        "minYAdjustment", 
        "minZAdjustment", 
        "maxXAdjustment", 
        "maxYAdjustment", 
        "maxZAdjustment"
        });

      for(const auto& id: ids) {
        adjustment = 
          Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
              builder->get_object(id));

        this->handlers.push_back(
          adjustment->signal_value_changed().connect(
            sigc::mem_fun(*this, &CropObserver::crop_changed)
            )
          );
      }

    }


    /*-----------------------------------------------------------------------*/

    ~CropObserver() {
      for( auto& handler: this->handlers) {
        handler.disconnect();
      }
    }

    /*-----------------------------------------------------------------------*/

    void crop_changed() {
      CreateInternalPlot action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    std::vector<sigc::connection> handlers;
};

#endif
