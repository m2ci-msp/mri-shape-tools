#ifndef __HSV2RGB_H__
#define __HSV2RGB_H__

class HSV2RGB {

public:

  /*-------------------------------------------------------------------------*/

  static void convert(
    double h,
    double s,
    double v,
    double& r,
    double& g,
    double& b
    ) {

    // scale hue to [0, 360]
    h *= 360;

    int hi = h / 60;

    double f = h / 60 - hi;

    double p = v * ( 1 - s );
    double q = v * ( 1 - s * f);
    double t = v * ( 1 - s * ( 1 - f));

    switch( hi ) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
    case 6:
      r = v;
      g = t;
      b = p;
      break;
    default:
      r = 0;
      g = 0;
      b = 0;
      break;
    }

  }

  /*-------------------------------------------------------------------------*/

};

#endif
