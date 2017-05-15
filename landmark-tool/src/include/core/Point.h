#ifndef __POINT_H__
#define __POINT_H__

#include <memory>
#include <array>

#include "singleton/CoordinateSystem.h"

class Point {

  public:

    Point() :
      system(CoordinateSystem::get_instance()),
      point({ 0, 0, 0}) {
      }

    Point(double x, double y, double z) :
      system(CoordinateSystem::get_instance()) {

        const int X = system->get_index(Axis::X);
        const int Y = system->get_index(Axis::Y);
        const int Z = system->get_index(Axis::Z);

        this->point[X] = x;
        this->point[Y] = y;
        this->point[Z] = z;

      }

    /** set and get coordinates for current coordinate system */
    void set_x(const double& x) {
      const int index = system->get_index(Axis::X);
      this->point[index] = x;
    }

    void set_y(const double& y) {
      const int index = system->get_index(Axis::Y);
      this->point[index] = y;
    }

    void set_z(const double& z) {
      const int index = system->get_index(Axis::Z);
      this->point[index] = z;
    }

    const double& get_x() const {
      const int index = system->get_index(Axis::X);
      return this->point.at(index);
    }

    const double& get_y() const {
      const int index = system->get_index(Axis::Y);
      return this->point.at(index);
    }

    const double& get_z() const {
      const int index = system->get_index(Axis::Z);
      return this->point.at(index);
    }

    /** set and get coordinate in the XYZ coordinate system */
    void set_canonical_x(const double& x) {
      this->point[0] = x;
    }

    void set_canonical_y(const double& y) {
      this->point[1] = y;
    }

    void set_canonical_z(const double& z) {
      this->point[2] = z;
    }

    const double& get_canonical_x() const {
      return this->point.at(0);
    }

    const double& get_canonical_y() const {
      return this->point.at(1);
    }

    const double& get_canonical_z() const {
      return this->point.at(2);
    }

    bool operator<= (const Point& other) const {
      return (
          get_canonical_x() <= other.get_canonical_x() &&
          get_canonical_y() <= other.get_canonical_y() &&
          get_canonical_z() <= other.get_canonical_z()
          );
    }

    bool operator>= (const Point& other) const {
      return (
          get_canonical_x() >= other.get_canonical_x() &&
          get_canonical_y() >= other.get_canonical_y() &&
          get_canonical_z() >= other.get_canonical_z()
          );
    }

    void scale(double scale) {

      for(auto& elem: this->point) {
        elem *= scale;
      }

    }

    /** computes Euclidian distance */
    double distance_to(const Point& other) const {
      return sqrt(
          pow(get_canonical_x() - other.get_canonical_x(), 2) +
          pow(get_canonical_y() - other.get_canonical_y(), 2) +
          pow(get_canonical_z() - other.get_canonical_z(), 2)
          );
    }

  private:

    std::shared_ptr<CoordinateSystem> system;
    std::array<double, 3> point;
};
#endif
