#ifndef __TRACKER_STATE_H__
#define __TRACKER_STATE_H__

#include <vector>
#include <armadillo>

class TrackerState{

public:

  bool firstFrame = true;
  bool sourceIdsSet = false;

  bool isPCA = false;

};

#endif
