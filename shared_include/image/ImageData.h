#ifndef __IMAGE_DATA_H__
#define __IMAGE_DATA_H__

#include <vector>

class ImageData{

public:

    // dimensions of image
    int nx;
    int ny;
    int nz;

    // origin of image
    double originX = 0;
    double originY = 0;
    double originZ = 0;
    
    // boundary sizes of image
    int bx = 0;
    int by = 0;
    int bz = 0;

    // grid spacings of image
    double hx = 0;
    double hy = 0;
    double hz = 0;

    // data storage
    std::vector<double> values;

};

#endif
