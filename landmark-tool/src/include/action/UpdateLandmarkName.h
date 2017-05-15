#ifndef __UPDATE_LANDMARK_NAME_H__
#define __UPDATE_LANDMARK_NAME_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"

/** Updates the name of a landmark */
class UpdateLandmarkName : public Action {

  public:
    /*----------------------------------------------------------------------*/
    UpdateLandmarkName( const Glib::ustring& pathString,
        const Glib::ustring& name ) : name(name) {

        // get builder object
        Glib::RefPtr<Gtk::Builder> builder =
          GuiDescription::get_instance()->get_builder();

        Glib::RefPtr<Gtk::ListStore> listStore = 
          Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
              builder->get_object("landmarkListStore"));

        Gtk::TreePath path = Gtk::TreePath(pathString);
        this->row = listStore->get_iter(path);

      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {
      if(row != nullptr) {
        // get id of changed landmark
        int id;
        row->get_value(1, id);

        // set name 
        LandmarkPool::get_instance()->get_landmark_by_id(id)->set_name(
            this->name);

        // set name in ui
        row->set_value(0, this->name);
      }
    }

    /*----------------------------------------------------------------------*/

  private:
    std::string name;
    Gtk::TreeModel::iterator row;

};
#endif
