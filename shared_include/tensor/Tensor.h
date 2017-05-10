#ifndef __TENSOR_H__
#define __TENSOR_H__

#include "tensor/TensorData.h"
#include "tensor/TensorAccess.h"
#include "tensor/TensorModes.h"
#include "tensor/TensorOperations.h"
#include "tensor/TensorTruncator.h"

class Tensor{

public:

  /*----------------------------------------------------------------------*/

  Tensor() {

    this->tensorAccess = nullptr;
    this->tensorModes = nullptr;
    this->tensorOperations = nullptr;
    this->tensorTruncator = nullptr;
    this->valid = false;

  }

  /*----------------------------------------------------------------------*/

  ~Tensor() {

    cleanup();

  }

  /*----------------------------------------------------------------------*/

  void cleanup() {

    if(this->valid == true) {
      delete this->tensorAccess;
      delete this->tensorModes;
      delete this->tensorOperations;
      delete this->tensorTruncator;
    }

    this->valid = false;

  }

  /*----------------------------------------------------------------------*/


  Tensor(const TensorData& data) : tensorData(data) {

    this->tensorAccess = new TensorAccess(this->tensorData);
    this->tensorModes = new TensorModes(this->tensorData, *this->tensorAccess);
    this->tensorOperations = new TensorOperations(*this->tensorModes);
    this->tensorTruncator =
      new TensorTruncator(
        this->tensorData, *this->tensorAccess);

    this->valid = true;

  }

  /*----------------------------------------------------------------------*/

  Tensor(const Tensor& tensor) : tensorData(tensor.tensorData) {

    cleanup();

    this->tensorAccess = new TensorAccess(this->tensorData);
    this->tensorModes = new TensorModes(this->tensorData, *this->tensorAccess);
    this->tensorOperations = new TensorOperations(*this->tensorModes);
    this->tensorTruncator =
      new TensorTruncator(
        this->tensorData, *this->tensorAccess);
    this->valid = true;

  }

  /*----------------------------------------------------------------------*/

  TensorData& data() {
    return this->tensorData;
  }

  /*----------------------------------------------------------------------*/

  const TensorData& data() const {
    return this->tensorData;
  }

  /*----------------------------------------------------------------------*/

  Tensor& operator=(const Tensor& other) {

    cleanup();

    this->tensorData = other.tensorData;
    this->tensorAccess = new TensorAccess(this->tensorData);
    this->tensorModes = new TensorModes(this->tensorData, *this->tensorAccess);
    this->tensorOperations = new TensorOperations(*this->tensorModes);
    this->tensorTruncator =
      new TensorTruncator(
        this->tensorData, *this->tensorAccess);

    this->valid = true;

    return *this;

  }

  /*----------------------------------------------------------------------*/

  TensorAccess& access() const {
    return *this->tensorAccess;

  }

  /*----------------------------------------------------------------------*/

  TensorModes& modes() const {
    return *this->tensorModes;
  }

  /*----------------------------------------------------------------------*/

  TensorOperations& operations() const {
    return *this->tensorOperations;
  }

  /*----------------------------------------------------------------------*/

  TensorTruncator& truncate() const {
    return *this->tensorTruncator;
  }

  /*----------------------------------------------------------------------*/

private:

  /*----------------------------------------------------------------------*/

  TensorData tensorData;
  TensorAccess* tensorAccess;
  TensorModes* tensorModes;
  TensorOperations* tensorOperations;
  TensorTruncator* tensorTruncator;

  bool valid = false;

  /*----------------------------------------------------------------------*/

};

#endif
