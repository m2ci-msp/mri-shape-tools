#ifndef __RESET_IMAGE_H__
#define __RESET_IMAGE_H__

#include "action/Action.h"
#include "core/ImageStackBuilder.h"
#include "singleton/Data.h"
#include "singleton/GuiDescription.h"

class ResetImage : public Action {

  public:

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      Image image = Data::get_instance()->get_original_image();
      Data::get_instance()->set_current_image(image);

      ImageStackBuilder builder;
      Data::get_instance()->set_image_stack(builder.build_from(image));

    }

    /*----------------------------------------------------------------------*/

};
#endif
