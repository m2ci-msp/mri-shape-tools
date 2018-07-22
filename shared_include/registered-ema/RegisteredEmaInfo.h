#ifndef __REGISTERED_EMA_INFO_H__
#define __REGISTERED_EMA_INFO_H__

#include "RegisteredEmaData.h"

class RegisteredEmaInfo{

private:

  RegisteredEmaData& data;

public:

  RegisteredEmaInfo(RegisteredEmaData& data) :

    data(data) {

  }

  const std::vector<double>& time_stamps() const {

    return this->data.timeStamps;

  }

  int sample_amount() const {

    return this->data.timeStamps.size();

  }

};
#endif
