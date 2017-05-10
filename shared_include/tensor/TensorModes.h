#ifndef __TENSOR_MODES_H__
#define __TENSOR_MODES_H__

#include <vector>

#include <armadillo>

#include "tensor/TensorData.h"
#include "tensor/TensorAccess.h"

class TensorModes{

public:

  /*---------------------------------------------------------------------------*/

  TensorModes(TensorData& tensorData, TensorAccess& tensorAccess) :
    tensorData(tensorData), tensorAccess(tensorAccess) {

  }

  /*---------------------------------------------------------------------------*/

  // extracts the list of the modeThreeDimension x modeTwoDimension matrices from
  // the mode three unfolding
  std::vector<arma::mat> get_mode_three_matrices_along_mode_one() {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    std::vector<arma::mat> result;

    for( int indexModeOne = 0; indexModeOne < modeOneDimension; ++indexModeOne) {

      arma::mat matrix(modeThreeDimension, modeTwoDimension);

      for(int indexModeTwo = 0; indexModeTwo < modeTwoDimension; ++indexModeTwo) {

        for(int indexModeThree = 0;
            indexModeThree < modeThreeDimension; ++indexModeThree) {

          matrix(indexModeThree, indexModeTwo) =
            this->tensorAccess.get_entry(
              indexModeOne, indexModeTwo, indexModeThree
              );

        } // end for indexModeThree

      } // end for indexModeTwo

      result.push_back(matrix);

    } // end for indexModeOne

    return result;

  }
  /*---------------------------------------------------------------------------*/

  // extracts the list of the modeThreeDimension x modeOneDimension matrices from
  // the mode three unfolding
  std::vector<arma::mat> get_mode_three_matrices_along_mode_two() {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    std::vector<arma::mat> result;

    for( int indexModeTwo = 0; indexModeTwo < modeTwoDimension; ++indexModeTwo) {

      arma::mat matrix(modeThreeDimension, modeOneDimension);

      for(int indexModeOne = 0; indexModeOne < modeOneDimension; ++indexModeOne) {

        for(int indexModeThree = 0;
            indexModeThree < modeThreeDimension; ++indexModeThree) {

          matrix(indexModeThree, indexModeOne) =
            this->tensorAccess.get_entry(
              indexModeOne, indexModeTwo, indexModeThree
              );

        } // end for indexModeThree

      } // end for indexModeOne

      result.push_back(matrix);

    } // end for indexModeTwo

    return result;

  }

  /*---------------------------------------------------------------------------*/

  arma::mat get_mode_one_matrix() {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    arma::mat matrix(modeOneDimension, modeTwoDimension * modeThreeDimension);

    for(int j = 0; j < modeTwoDimension; ++j) {

      for(int k = 0; k < modeThreeDimension; ++k) {

        const int columnIndex = j * modeThreeDimension + k;

        for(int i = 0; i < modeOneDimension; ++i) {

          const int rowIndex = i;

          matrix(rowIndex, columnIndex) =
            this->tensorAccess.get_entry(i, j, k);

        } // end i
      } // end k
    } // end j


    return matrix;

  }

  /*---------------------------------------------------------------------------*/

  arma::mat get_mode_two_matrix() {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    arma::mat matrix(modeTwoDimension, modeOneDimension * modeThreeDimension);

    for(int k = 0; k < modeThreeDimension; ++k) {

      for(int i = 0; i < modeOneDimension; ++i) {

        const int columnIndex = k * modeOneDimension + i;

        for(int j = 0; j < modeTwoDimension; ++j) {

          const int rowIndex = j;

          matrix(rowIndex, columnIndex) =
            this->tensorAccess.get_entry(i, j, k);

        } // end j
      } // end i
    } // end k

    return matrix;

  }

  /*---------------------------------------------------------------------------*/

  arma::mat get_mode_three_matrix() {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    arma::mat matrix(modeThreeDimension, modeOneDimension * modeTwoDimension);

    for(int i = 0; i < modeOneDimension; ++i) {
      for(int j = 0; j < modeTwoDimension; ++j) {

        const int columnIndex = i * modeTwoDimension + j;

        for(int k = 0; k < modeThreeDimension; ++k) {

          const int rowIndex = k;

          matrix(rowIndex, columnIndex) =
            this->tensorAccess.get_entry(i, j, k);

        } // end k
      } // end i
    } // end j


    return matrix;

  }


  /*---------------------------------------------------------------------------*/

  void unfold_to_mode_one_vectors(const arma::mat& modeOneMatrix) {

    int modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    if(
      (int) modeOneMatrix.n_cols > modeTwoDimension * modeThreeDimension ||
      (int) modeOneMatrix.n_rows > modeOneDimension
      ) {
      throw std::runtime_error("Unfolding error: Dimensions too large.");
    }

    modeOneDimension = modeOneMatrix.n_rows;

    std::vector<double> newData;

    for(int i = 0; i < modeOneDimension; ++i) {

      const int rowIndex = i;

      for(int j = 0; j < modeTwoDimension; ++j) {
        for(int k = 0; k < modeThreeDimension; ++k) {

          const int columnIndex = j * modeThreeDimension + k;

          newData.push_back(modeOneMatrix(rowIndex, columnIndex));

        } // end for k
      } // end for j
    } // end for i

    this->tensorData.set_data(newData);
    this->tensorData.set_mode_dimensions(
      modeOneDimension, modeTwoDimension, modeThreeDimension
      );
    
  }

  /*---------------------------------------------------------------------------*/

  void unfold_to_mode_two_vectors(const arma::mat& modeTwoMatrix) {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    int modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    if(
      (int) modeTwoMatrix.n_cols > modeOneDimension * modeThreeDimension ||
      (int) modeTwoMatrix.n_rows > modeTwoDimension
      ) {
      throw std::runtime_error("Unfolding error: Dimensions too large.");
    }

    modeTwoDimension = modeTwoMatrix.n_rows;

    std::vector<double> newData;

    for(int i = 0; i < modeOneDimension; ++i) {

      for(int j = 0; j < modeTwoDimension; ++j) {

        const int rowIndex = j;

        for(int k = 0; k < modeThreeDimension; ++k) {

          const int columnIndex = k * modeOneDimension + i;

          newData.push_back(modeTwoMatrix(rowIndex, columnIndex));

        } // end for k
      } // end for j
    } // end for i

    this->tensorData.set_data(newData);
    this->tensorData.set_mode_dimensions(
      modeOneDimension, modeTwoDimension, modeThreeDimension
      );

  }

  /*---------------------------------------------------------------------------*/

  void unfold_to_mode_three_vectors(const arma::mat& modeThreeMatrix) {

    const int& modeOneDimension = tensorData.get_mode_one_dimension();
    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    int modeThreeDimension = tensorData.get_mode_three_dimension();

    if(
      (int) modeThreeMatrix.n_cols > modeOneDimension * modeTwoDimension ||
      (int) modeThreeMatrix.n_rows > modeThreeDimension
      ) {
      throw std::runtime_error("Unfolding error: Dimensions too large.");
    }

    modeThreeDimension = modeThreeMatrix.n_rows;

    std::vector<double> newData;

    for(int i = 0; i < modeOneDimension; ++i) {

      for(int j = 0; j < modeTwoDimension; ++j) {

        const int columnIndex = i * modeTwoDimension + j;

        for(int k = 0; k < modeThreeDimension; ++k) {

          const int rowIndex = k;

          newData.push_back(modeThreeMatrix(rowIndex, columnIndex));

        } // end for k
      } // end for j
    } // end for i

    this->tensorData.set_data(newData);
    this->tensorData.set_mode_dimensions(
      modeOneDimension, modeTwoDimension, modeThreeDimension
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
