#ifndef __EMA_HEAD_CORRECTION_H__
#define __EMA_HEAD_CORRECTION_H__

#include <string>

#include <armadillo>

#include "ema/EmaCoil.h"
#include "ema/EmaCoordinateSystem.h"

/* given reference coils, the orientation of the coordinate system is:
 *
 * x-axis: left to right
 * y-axis: front to back
 * z-axis: bottom to top
 *
 */
class EmaHeadCorrection{

private:

  // names of the reference coils
  std::string leftCoilName;
  std::string rightCoilName;
  std::string frontCoilName;

  // reference to main EmaData instance
  EmaData& emaData;

public:

  EmaHeadCorrection(EmaData& emaData) : emaData(emaData) {

  }

  void with(
            const std::string& leftCoilName,
            const std::string& rightCoilName,
            const std::string& frontCoilName
            ) {

    this->leftCoilName = leftCoilName;
    this->rightCoilName = rightCoilName;
    this->frontCoilName = frontCoilName;

    for(size_t i = 0; i < this->emaData.emaInfoData.timeStamps.size(); ++i) {

      arma::vec translation;
      arma::mat mappingMatrix;

      const EmaCoordinateSystem system = construct_mapping(i);

      apply_mapping(system, i);

    }

  }

private:

  EmaCoordinateSystem construct_mapping(const int& sampleIndex) const {

    const arma::vec left = emaData.emaCoils.at(this->leftCoilName).access().position(sampleIndex);
    const arma::vec right = emaData.emaCoils.at(this->rightCoilName).access().position(sampleIndex);
    const arma::vec front = emaData.emaCoils.at(this->frontCoilName).access().position(sampleIndex);

    EmaCoordinateSystem system;
    system.build_from(left, right, front);

    return system;

  }

  void apply_mapping(const EmaCoordinateSystem& system, const int& sampleIndex) {

    for(auto& mapObject: this->emaData.emaCoils) {

      EmaCoil& coil = mapObject.second;

      coil.access().position(sampleIndex) = system.map( coil.access().position(sampleIndex) );

    }

  }

};
#endif
