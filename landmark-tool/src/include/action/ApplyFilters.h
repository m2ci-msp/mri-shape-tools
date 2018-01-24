#ifndef __APPLY_FILTERS_H__
#define __APPLY_FILTERS_H__

#include "action/Action.h"
#include "core/ImageStackBuilder.h"
#include "singleton/Data.h"
#include "singleton/GuiDescription.h"
#include "image-modify/ApplyModifications.h"

class ApplyFilters : public Action {

  public:
    /*----------------------------------------------------------------------*/

    ApplyFilters() {

      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->filtersText = 
        Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(
            builder->get_object("filterTextBuffer"));

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      Image image = Data::get_instance()->get_current_image();
      ApplyModifications(image).apply(filtersText->get_text());
      Data::get_instance()->set_current_image(image);

      ImageStackBuilder builder;
      Data::get_instance()->set_image_stack(builder.build_from(image));

    }

    /*----------------------------------------------------------------------*/

  private:

    Glib::RefPtr<Gtk::TextBuffer> filtersText;

};
#endif
