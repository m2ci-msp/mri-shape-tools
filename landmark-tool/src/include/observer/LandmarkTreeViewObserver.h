#ifndef __LANDMARK_TREE_VIEW_OBSERVER_H__
#define __LANDMARK_TREE_VIEW_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/SelectLandmark.h"
#include "action/UpdateLandmarkName.h"
#include "action/DeleteLandmark.h"

/** Class that watches the state of the openImageStackButton */
class LandmarkTreeViewObserver {
  public:

    /*-----------------------------------------------------------------------*/

    LandmarkTreeViewObserver() {
      // get builder from gui description
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      builder->get_widget("landmarkTreeView", this->view);

      this->landmarkSelectedHandler = view->signal_cursor_changed().connect(
          sigc::mem_fun(*this, &LandmarkTreeViewObserver::landmark_selected)
          );

      Glib::RefPtr<Gtk::CellRendererText> nameColumn =
        Glib::RefPtr<Gtk::CellRendererText>::cast_dynamic(
            builder->get_object("landmarkNameRenderer"));


      this->landmarkEditedHandler = nameColumn->signal_edited().connect(
          sigc::mem_fun(*this, &LandmarkTreeViewObserver::landmark_edited)
          );

      this->landmarkEditingStartedHandler =
        nameColumn->signal_editing_started().connect(
            sigc::mem_fun(*this,
              &LandmarkTreeViewObserver::landmark_editing_started)
            );

      this->landmarkEditingCanceledHandler =
        nameColumn->signal_editing_canceled().connect(
            sigc::mem_fun(*this,
              &LandmarkTreeViewObserver::landmark_editing_canceled)
            );

      this->keyReleaseHandler = view->signal_key_release_event().connect(
          sigc::mem_fun(*this, &LandmarkTreeViewObserver::key_released)
          );

      this->editing_active = false;

    }


    /*-----------------------------------------------------------------------*/

    ~LandmarkTreeViewObserver() {
      this->landmarkSelectedHandler.disconnect();
      this->landmarkEditedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/
    void landmark_edited( const Glib::ustring& pathString,
        const Glib::ustring& name) {

      this->editing_active = false;
      UpdateLandmarkName action( pathString, name);
      action.execute();

    }
    /*-----------------------------------------------------------------------*/

    void landmark_selected() {
      SelectLandmark action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

    bool key_released( GdkEventKey* event ) {
      // delete currently selected row if the DEL key was released
      if( this->editing_active == false ) {
        if( event->keyval == GDK_KEY_Delete ) {
          DeleteLandmark action;
          action.execute();

          return true;
        }
      }

      return false;
    }

    /*-----------------------------------------------------------------------*/
    void landmark_editing_started( Gtk::CellEditable*,
        const Glib::ustring&) {
      this->editing_active = true;
    }

    /*-----------------------------------------------------------------------*/

    void landmark_editing_canceled() {
      this->editing_active = false;
    }

    /*-----------------------------------------------------------------------*/
  private:
    sigc::connection landmarkSelectedHandler;
    sigc::connection landmarkEditedHandler;
    sigc::connection landmarkEditingStartedHandler;
    sigc::connection landmarkEditingCanceledHandler;
    sigc::connection keyReleaseHandler;

    bool editing_active;
    Gtk::TreeView* view;
};

#endif
