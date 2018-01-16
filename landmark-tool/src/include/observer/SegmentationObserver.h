#ifndef __SEGMENTATION_OBSERVER_H__
#define __SEGMENTATION_OBSERVER_H__

#include "singleton/Data.h"
#include "action/CreateInternalPlot.h"

class SegmentationObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SegmentationObserver() {

      // connect handler to clicked signal
      this->stackSetHandler = Data::get_instance()->segmentation_stack_set.connect(
          sigc::mem_fun(*this, &SegmentationObserver::data_changed)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~SegmentationObserver() {
      this->stackSetHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void data_changed() {

      CreateInternalPlot createPlot;
      createPlot.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection stackSetHandler;
};

#endif
