#ifndef __SUBSET_H__
#define __SUBSET_H__

#include <string>
#include <vector>

#include "ErrorDatabase.h"

class Subset{

public:

  /*-----------------------------------------------------------------------*/

  Subset(
    const std::string& name,
    const std::vector<int>& indices) :
    name(name), indices(indices) {
  }

  /*-----------------------------------------------------------------------*/

  const std::string name;
  const std::vector<int> indices;
  ErrorDatabase errorDataBase;

  /*-----------------------------------------------------------------------*/

};

#endif
