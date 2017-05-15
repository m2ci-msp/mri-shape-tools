#ifndef __DRAWING_AREA_OBSERVER_H__
#define __DRAWING_AREA_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "singleton/EventPosition.h"
#include "singleton/Data.h"
#include "singleton/EditMode.h"

#include "action/DisplayPlot.h"
#include "action/ConfigureScrollbars.h"
#include "action/SetEventPosition.h"
#include "action/AddLandmark.h"
#include "action/MoveLandmark.h"
#include "action/CreateInternalPlot.h"
#include "action/FindNearestLandmark.h"
#include "action/UpdateLandmarkNormal.h"

class DrawingAreaObserver {
  public:

    /*-----------------------------------------------------------------------*/

    DrawingAreaObserver() :
      mode(EditMode::get_instance()) {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();
      Gtk::DrawingArea* area;

      builder->get_widget("mainDrawingArea", area);


      this->redrawHandler = area->signal_draw().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::redraw_triggered)
          );

      this->resizeHandler = area->signal_size_allocate().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::resize_triggered)
          );

      this->buttonPressHandler = area->signal_button_press_event().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::button_pressed)
          );

      this->motionHandler = area->signal_motion_notify_event().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::motion_occured)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~DrawingAreaObserver() {
      this->redrawHandler.disconnect();
      this->resizeHandler.disconnect();
      this->buttonPressHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    bool redraw_triggered(const Cairo::RefPtr<Cairo::Context>&) {
      DisplayPlot action;
      action.execute();
      return false;
    }

    /*-----------------------------------------------------------------------*/

    void resize_triggered( Gdk::Rectangle&) {
      ConfigureScrollbars action;
      action.execute();
    }


    /*-----------------------------------------------------------------------*/

    bool motion_occured(GdkEventMotion *event) {

      if(
          this->mode->get_mode() == Mode::EDIT_NORMAL &&
          LandmarkPool::get_instance()->get_selected_landmark() != nullptr ) {

        // compute internal coordinates from cursor
        SetEventPosition setPosition(event->x, event->y);
        setPosition.execute();

        UpdateLandmarkNormal action;
        action.execute();

        CreateInternalPlot plot;
        plot.execute();

      }

      return true;

    }

    /*-----------------------------------------------------------------------*/

    bool button_pressed ( GdkEventButton* event) {

      if(!Data::get_instance()->stack_present()) {
        return false;
      }

      // compute internal coordinates from cursor
      SetEventPosition setPosition(event->x, event->y);
      setPosition.execute();

      switch(this->mode->get_mode()) {
        case Mode::STANDARD:
          button_pressed_standard_mode( event );
          break;

        case Mode::MOVE_LANDMARK:
          button_pressed_move_mode( event );
          break;

        case Mode::EDIT_NORMAL:
          button_pressed_normal_mode( event );
          break;

      }

      return false;
    }

    /*-----------------------------------------------------------------------*/

  private:

    /*-----------------------------------------------------------------------*/

    void button_pressed_standard_mode ( GdkEventButton* event) {
      if( event->button == 3) {
        FindNearestLandmark action;
        action.execute();
        Landmark* found = action.get_result();
        if( found ) {
          LandmarkPool::get_instance()->select_id(found->get_id());
        }
      }
      else {
        AddLandmark addLandmark;
        addLandmark.execute();
      }

    }

    /*-----------------------------------------------------------------------*/

    void button_pressed_move_mode(GdkEventButton* event) {
      if( event->button == 3) {
        FindNearestLandmark action;
        action.execute();
        Landmark* found = action.get_result();
        if( found ) {
          LandmarkPool::get_instance()->select_id(found->get_id());
        }
      }

      else if(LandmarkPool::get_instance()->get_selected_landmark() != nullptr) {

        MoveLandmark moveLandmark;
        moveLandmark.execute();

        CreateInternalPlot action;
        action.execute();

      }
    }

    /*-----------------------------------------------------------------------*/

    void button_pressed_normal_mode(GdkEventButton*) {
      this->mode->set_mode(Mode::STANDARD);
    }

    /*-----------------------------------------------------------------------*/

    std::shared_ptr<EditMode> mode;

    sigc::connection redrawHandler;
    sigc::connection resizeHandler;
    sigc::connection buttonPressHandler;
    sigc::connection motionHandler;
};

#endif
