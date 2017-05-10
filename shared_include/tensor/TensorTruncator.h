#ifndef __TENSOR_TRUNCATOR_H__
#define __TENSOR_TRUNCATOR_H__

#include <set>

#include "tensor/TensorData.h"
#include "tensor/TensorAccess.h"
#include "tensor/TensorModes.h"

class TensorTruncator{

public:

  /*---------------------------------------------------------------------------*/

  TensorTruncator(
    TensorData& tensorData,
    TensorAccess& tensorAccess) : tensorData(tensorData),
                                tensorAccess(tensorAccess) {
  }

  /*---------------------------------------------------------------------------*/

  /*
  void mode_one(const std::set<int>& indicesModeOne) {

    int dimensionModeTwo = this->tensorData.get_mode_two_dimension();
    int dimensionModeThree = this->tensorData.get_mode_three_dimension();

    std::set<int> indicesModeTwo;
    std::set<int> indicesModeThree;

    for(int i = 0; i < dimensionModeTwo;   ++i) { indicesModeTwo.insert(i);   }
    for(int i = 0; i < dimensionModeThree; ++i) { indicesModeThree.insert(i); }

    modes(indicesModeOne, indicesModeTwo, indicesModeThree);

  }
  */

  /*---------------------------------------------------------------------------*/

  /*
  void mode_two(const std::set<int>& indicesModeTwo) {

    int dimensionModeOne = this->tensorData.get_mode_one_dimension();
    int dimensionModeThree = this->tensorData.get_mode_three_dimension();

    std::set<int> indicesModeOne;
    std::set<int> indicesModeThree;

    for(int i = 0; i < dimensionModeOne;   ++i) { indicesModeOne.insert(i);   }
    for(int i = 0; i < dimensionModeThree; ++i) { indicesModeThree.insert(i); }

    modes(indicesModeOne, indicesModeTwo, indicesModeThree);

  }
  */

  /*---------------------------------------------------------------------------*/

  void mode_three(const std::set<int>& indicesModeThree) {

    int dimensionModeOne = this->tensorData.get_mode_one_dimension();
    int dimensionModeTwo = this->tensorData.get_mode_two_dimension();

    std::set<int> indicesModeOne;
    std::set<int> indicesModeTwo;

    for(int i = 0; i < dimensionModeOne; ++i) { indicesModeOne.insert(i); }
    for(int i = 0; i < dimensionModeTwo; ++i) { indicesModeTwo.insert(i); }

    modes(indicesModeOne, indicesModeTwo, indicesModeThree);

  }

  /*---------------------------------------------------------------------------*/


  void modes(
    const std::set<int>& indicesModeOne,
    const std::set<int>& indicesModeTwo,
    const std::set<int>& indicesModeThree
    ) {

    if(indicesModeOne.empty() ||
       indicesModeTwo.empty() ||
       indicesModeThree.empty() ) {

      throw std::runtime_error("Aborting truncation: Tensor would be empty.");

    }

    std::vector<double> newData;

    for(const int& indexModeOne: indicesModeOne) {

      for(const int& indexModeTwo: indicesModeTwo) {

        for(const int& indexModeThree: indicesModeThree ) {

          newData.push_back(
            this->tensorAccess.get_entry(
                indexModeOne, indexModeTwo, indexModeThree)
            );

        } // end for indexModeThree

      } // end for indexModeTwo

    } // end for indexModeOne

    this->tensorData.set_data(newData);

    this->tensorData.set_mode_dimensions(
      indicesModeOne.size(),
      indicesModeTwo.size(),
      indicesModeThree.size()
      );

  }

  /*---------------------------------------------------------------------------*/

private:

  /*---------------------------------------------------------------------------*/

  TensorData& tensorData;
  TensorAccess& tensorAccess;

  /*---------------------------------------------------------------------------*/

};


#endif
