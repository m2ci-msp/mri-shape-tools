#ifndef __CLEAR_LANDMARKS_H__
#define __CLEAR_LANDMARKS_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"

/** Removes all landmarks */
class ClearLandmarks : public Action {

  public:
    /*----------------------------------------------------------------------*/

    ClearLandmarks() {
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->listStore = 
        Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
            builder->get_object("landmarkListStore"));
    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      LandmarkPool::get_instance()->clear(); 

      this->listStore->clear();

    }

    /*----------------------------------------------------------------------*/

  private:
    Glib::RefPtr<Gtk::ListStore> listStore;
};
#endif
