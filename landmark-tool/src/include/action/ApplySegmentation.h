#ifndef __APPLY_SEGMENTATION_H__
#define __APPLY_SEGMENTATION_H__

#include "action/Action.h"
#include "core/SegmentationStackBuilder.h"
#include "singleton/Data.h"
#include "singleton/GuiDescription.h"
#include "image-modify/ApplyModifications.h"

class ApplySegmentation : public Action {

  public:
    /*----------------------------------------------------------------------*/

    ApplySegmentation() {

      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->segmentationText = 
        Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(
            builder->get_object("segmentationTextBuffer"));

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      Image image = Data::get_instance()->get_current_image();
      ApplyModifications(image).apply(segmentationText->get_text());

      SegmentationStackBuilder builder;
      Data::get_instance()->set_segmentation_stack(builder.build_from(image));

    }

    /*----------------------------------------------------------------------*/

  private:

    Glib::RefPtr<Gtk::TextBuffer> segmentationText;

};
#endif
