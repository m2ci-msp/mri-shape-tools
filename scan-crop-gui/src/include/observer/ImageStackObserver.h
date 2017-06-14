#ifndef __IMAGE_STACK_OBSERVER_H__
#define __IMAGE_STACK_OBSERVER_H__

#include "singleton/Data.h"
#include "action/ConfigureSliceAdjustment.h"
#include "action/CreateInternalPlot.h"
#include "action/ConfigureCropAdjustments.h"

class ImageStackObserver {
  public:

    /*-----------------------------------------------------------------------*/

    ImageStackObserver() {

      // connect handler to clicked signal
      this->stackSetHandler = Data::get_instance()->image_stack_set.connect(
          sigc::mem_fun(*this, &ImageStackObserver::data_changed)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~ImageStackObserver() {
      this->stackSetHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void data_changed() {
      ConfigureSliceAdjustment configure;
      configure.execute();

      ConfigureCropAdjustments configureCrop;
      configureCrop.execute();

      CreateInternalPlot createPlot;
      createPlot.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection stackSetHandler;
};

#endif
