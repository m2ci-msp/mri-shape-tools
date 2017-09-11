#ifndef __AXIS_ACCESS_H__
#define __AXIS_ACCESS_H__

#include <string>

class AxisAccess{

public:

  AxisAccess(const std::string specification) {

    determine_axis_access(specification);

  }

  int x() const { return this->indexX; }
  int y() const { return this->indexY; }
  int z() const { return this->indexZ; }

private:

  void determine_axis_access(const std::string& specification) {
  
    if( specification == "XYZ" ) {
  
      this->indexX = 0;
      this->indexY = 1;
      this->indexZ = 2;
  
    }
    else if ( specification == "ZXY" ) {
  
      this->indexX = 2;
      this->indexY = 0;
      this->indexZ = 1;
  
    }
    else if ( specification == "ZYX" ) {
  
      this->indexX = 2;
      this->indexY = 1;
      this->indexZ = 0;
  
    }
    else if ( specification == "YZX" ) {
  
      this->indexX = 1;
      this->indexY = 2;
      this->indexZ = 0;
  
    }
    else if ( specification == "YXZ" ) {
  
    this->indexX = 1;
    this->indexY = 0;
    this->indexZ = 2;
  
    }
    else if ( specification == "XZY" ) {
  
      this->indexX = 0;
      this->indexY = 2;
      this->indexZ = 1;
  
    } else {
  
      throw std::runtime_error("Unknown axis specification.");
  
    }
  
  }

  int indexX;
  int indexY;
  int indexZ;

};
#endif
