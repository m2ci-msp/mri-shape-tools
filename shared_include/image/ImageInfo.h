#ifndef __IMAGE_INFO_H__
#define __IMAGE_INFO_H__

#include "image/ImageData.h"

class ImageInfo{

public:

    // constructor
    ImageInfo(const ImageData& imageData) : imageData(imageData) {
    }

    int get_nx() const { return this->imageData.nx; }
    int get_ny() const { return this->imageData.ny; }
    int get_nz() const { return this->imageData.nz; }

    int get_bx() const { return this->imageData.bx; }
    int get_by() const { return this->imageData.by; }
    int get_bz() const { return this->imageData.bz; }

    double get_hx() const { return this->imageData.hx; }
    double get_hy() const { return this->imageData.hy; }
    double get_hz() const { return this->imageData.hz; }

    double get_org_x() const { return this->imageData.orgX; }
    double get_org_y() const { return this->imageData.orgY; }
    double get_org_z() const { return this->imageData.orgZ; }

private:

    const ImageData& imageData;

};
#endif
