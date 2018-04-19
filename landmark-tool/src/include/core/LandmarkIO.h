#ifndef LANDMARKIO__
#define LANDMARKIO__

#include <memory>
#include <list>
#include <fstream>
#include <stdexcept>
#include <utility>

#include <json/json.h>

#include "core/Point.h"
#include "singleton/LandmarkPool.h"

/** class for input and output operations of annotations represented as a list
 * of colored paths
 */
class LandmarkIO {
  public:
    /** reads landmarks from a given file
     * @param fileName File name of the file containing the landmark collection.
     */
    static void read_from_file(
        const std::string& fileName) {

      // try to open file
      std::ifstream inFile(fileName);

      // throw exception if file can not be opened
      if( inFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file.");
      }

      // get origin of scan
      const double orgX = Data::get_instance()->get_current_image().info().get_origin_x() / Data::get_instance()->get_current_image().info().get_hx();
      const double orgY = Data::get_instance()->get_current_image().info().get_origin_y() / Data::get_instance()->get_current_image().info().get_hy();
      const double orgZ = Data::get_instance()->get_current_image().info().get_origin_z() / Data::get_instance()->get_current_image().info().get_hz();

      Json::Value root;

      inFile >> root;
      inFile.close();

      for( const auto& value: root) {

        std::string name(value["name"].asString());

        // map to local coordinate system
        const double x = value["x"].asDouble() - orgX;
        const double y = value["y"].asDouble() - orgY;
        const double z = value["z"].asDouble() - orgZ;

        Point point;

        // set coordinates in canonical coordinate system
        point.set_canonical_x(x);
        point.set_canonical_y(y);
        point.set_canonical_z(z);

        // store landmark 
        LandmarkPool::get_instance()->create_landmark(name, point);

      }

    }

    /*-----------------------------------------------------------------------*/

    /** writes landmarks to a provided file.
     * @param fileName name of the file
     */
    static void write_to_file(const std::string& fileName) {

      // try to open file
      std::ofstream outFile(fileName);

      // throw exception if file can not be opened
      if( outFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file.");
      }

      // get origin of scan
      const double orgX = Data::get_instance()->get_current_image().info().get_origin_x() / Data::get_instance()->get_current_image().info().get_hx();
      const double orgY = Data::get_instance()->get_current_image().info().get_origin_y() / Data::get_instance()->get_current_image().info().get_hy();
      const double orgZ = Data::get_instance()->get_current_image().info().get_origin_z() / Data::get_instance()->get_current_image().info().get_hz();

      // create root node of json file
      Json::Value root(Json::arrayValue);

      // output landmarks
      for(
          const auto& mark: LandmarkPool::get_instance()->get_all_landmarks()) {


        // get coordinates
        auto point = mark->get_position();

        // map to global coordinate system
        const double x = point.get_canonical_x() + orgX;
        const double y = point.get_canonical_y() + orgY;
        const double z = point.get_canonical_z() + orgZ;

        Json::Value landmark(Json::objectValue);

        Json::Value name(mark->get_name());
        Json::Value posX(x);
        Json::Value posY(y);
        Json::Value posZ(z);

        landmark["name"] = Json::Value(mark->get_name());
        landmark["x"]    = Json::Value(x);
        landmark["y"]    = Json::Value(y);
        landmark["z"]    = Json::Value(z);

        root.append(landmark);

      } // end for

      outFile << root << std::endl;
      outFile.close();

    }
  private:
    // constructor should not be used
    LandmarkIO() {
    }
};

#endif
