#ifndef __IMAGE_DATA_H__
#define __IMAGE_DATA_H__

#include <vector>

class ImageData{

public:

    // origin of image
    double orgX = 0;
    double orgY = 0;
    double orgZ = 0;
    
    // resolution of image
    int nx;
    int ny;
    int nz;
    
    // boundary sizes
    int bx = 0;
    int by = 0;
    int bz = 0;
    
    // grid spacings
    double hx = 1;
    double hy = 1;
    double hz = 1;
    
    // data storage
    std::vector<double> values;

};

#endif
