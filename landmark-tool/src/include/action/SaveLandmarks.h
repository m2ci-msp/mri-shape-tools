#ifndef __SAVE_LANDMARKS_H__
#define __SAVE_LANDMARKS_H__

#include "action/Action.h"
#include "core/LandmarkIO.h"

/** Saves landmarks to file */
class SaveLandmarks : public Action {

  public:
    SaveLandmarks(const std::string& path) :
      path(path) {
      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {
      LandmarkIO::write_to_file(this->path);
    }

    /*----------------------------------------------------------------------*/

  private:
    std::string path;
};
#endif
