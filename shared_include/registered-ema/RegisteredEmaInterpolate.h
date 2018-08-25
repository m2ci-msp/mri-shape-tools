#ifndef __REGISTERED_EMA_INTERPOLATE_H__
#define __REGISTERED_EMA_INTERPOLATE_H__

#include "RegisteredEmaData.h"
#include "RegisteredEmaAccess.h"


class RegisteredEmaInterpolate{

private:

  RegisteredEmaData& data;
  RegisteredEmaAccess access;

public:

  RegisteredEmaInterpolate(RegisteredEmaData& data) :

    data(data),
    access(data) {

  }

  arma::vec speaker_weight_at_time(const double& timeStamp) const {

    const double timeFrame = convert_to_frame(timeStamp);

    return speaker_weight_at_frame(timeFrame);

  }

  arma::vec speaker_weight_at_frame(const double& timeFrame) const {

    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;

    // get values at participating frames
    const arma::vec& left = this->access.speaker_weight(integerPart);

    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {

      return left;

    }

    const arma::vec& right = this->access.speaker_weight(integerPart + 1);

    // interpolate
    return left + subIntegerPart * ( right - left );

  }

  arma::vec phoneme_weight_at_time(const double& timeStamp) const {

    const double timeFrame = convert_to_frame(timeStamp);

    return phoneme_weight_at_frame(timeFrame);

  }

  arma::vec phoneme_weight_at_frame(const double& timeFrame) const {

    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;

    // get values at participating frames
    const arma::vec& left = this->access.phoneme_weight(integerPart);

    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {

      return left;

    }

    const arma::vec& right = this->access.phoneme_weight(integerPart + 1);

    // interpolate
    return left + subIntegerPart * ( right - left );

  }

  arma::vec speaker_variation_at_frame(const double& timeFrame) const {

    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;

    // get values at participating frames
    const arma::vec& left = this->access.speaker_variation(integerPart);

    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {

      return left;

    }

    const arma::vec& right = this->access.speaker_variation(integerPart + 1);

    // interpolate
    return left + subIntegerPart * ( right - left );

  }

  arma::vec speaker_variation_at_time(const double& timeStamp) const {

    const double timeFrame = convert_to_frame(timeStamp);

    return speaker_variation_at_frame(timeFrame);

  }

  arma::vec phoneme_variation_at_frame(const double& timeFrame) const {

    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;

    // get values at participating frames
    const arma::vec& left = this->access.phoneme_variation(integerPart);

    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {

      return left;

    }

    const arma::vec& right = this->access.phoneme_variation(integerPart + 1);

    // interpolate
    return left + subIntegerPart * ( right - left );

  }

  arma::vec phoneme_variation_at_time(const double& timeStamp) const {

    const double timeFrame = convert_to_frame(timeStamp);

    return phoneme_variation_at_frame(timeFrame);

  }

  std::vector<arma::vec> target_points_at_time(const double& timeStamp) const {

    const double timeFrame = convert_to_frame(timeStamp);

    return target_points_at_frame(timeFrame);

  }

  std::vector<arma::vec> target_points_at_frame(const double& timeFrame) const {

    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;

    // get values at participating frames
    const std::vector<arma::vec>& left = this->access.target_points(integerPart);

    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {

      return left;

    }

    const std::vector<arma::vec>& right = this->access.target_points(integerPart + 1);

    std::vector<arma::vec> result = left;

    for(size_t i = 0; i < result.size(); ++i) {

      result[i] = left[i] + subIntegerPart * ( right[i] - left[i] );

    }

    return result;

  }

private:

  double convert_to_frame(const double& timeStamp) const {

    const double& startTime = *( this->data.timeStamps.begin() );
    const double& endTime = *( this->data.timeStamps.end() - 1 );

    const double totalDuration = endTime - startTime;

    // map to interval [0, 1]
    const double normalizedFrame = ( timeStamp - startTime ) / totalDuration;

    // rescale, make sure that last index is timeStamps.size() - 1
    return normalizedFrame * ( this->data.timeStamps.size() - 1);

  }

};
#endif
