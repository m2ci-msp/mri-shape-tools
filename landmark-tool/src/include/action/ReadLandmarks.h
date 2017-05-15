#ifndef __READ_LANDMARKS_H__
#define __READ_LANDMARKS_H__

#include "action/Action.h"
#include "core/LandmarkIO.h"

/** Read landmarks from file */
class ReadLandmarks : public Action {

  public:
    ReadLandmarks(const std::string& path) :
      path(path) {
      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {
      LandmarkIO::read_from_file(this->path);
    }

    /*----------------------------------------------------------------------*/

  private:
    std::string path;
};
#endif
