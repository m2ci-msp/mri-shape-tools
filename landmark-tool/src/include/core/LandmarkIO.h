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
      const double orgX = Data::get_instance()->get_scan().data()->get_org_x();
      const double orgY = Data::get_instance()->get_scan().data()->get_org_y();
      const double orgZ = Data::get_instance()->get_scan().data()->get_org_z();

      Json::Value root;
      Json::Reader reader;

      reader.parse(inFile, root);

      for( const auto& value: root) {

        std::string name(value["name"].asString());

        // map to local coordinate system
        const double x = value["x"].asDouble() - orgX;
        const double y = value["y"].asDouble() - orgY;
        const double z = value["z"].asDouble() - orgZ;

        const double nx = value["nx"].asDouble();
        const double ny = value["ny"].asDouble();
        const double nz = value["nz"].asDouble();

        Point point;
        Point normal;

        // set coordinates in canonical coordinate system
        point.set_canonical_x(x);
        point.set_canonical_y(y);
        point.set_canonical_z(z);

        normal.set_canonical_x(nx);
        normal.set_canonical_y(ny);
        normal.set_canonical_z(nz);

        // store landmark 
        auto mark = LandmarkPool::get_instance()->create_landmark(name, point);

        // set normal
        mark->set_normal(normal);

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
      const double orgX = Data::get_instance()->get_scan().data()->get_org_x();
      const double orgY = Data::get_instance()->get_scan().data()->get_org_y();
      const double orgZ = Data::get_instance()->get_scan().data()->get_org_z();

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

        // get normal
        auto normal = mark->get_normal();

        double nx = normal.get_canonical_x();
        double ny = normal.get_canonical_y();
        double nz = normal.get_canonical_z();

        // normalize
        const double length = sqrt(pow(nx, 2) + pow(ny, 2) + pow(nz, 2));
        if( length > 0) {
          nx /= length;
          ny /= length;
          nz /= length;
        }

        Json::Value landmark(Json::objectValue);

        Json::Value name(mark->get_name());
        Json::Value posX(x);
        Json::Value posY(y);
        Json::Value posZ(z);

        landmark["name"] = Json::Value(mark->get_name());
        landmark["x"]    = Json::Value(x);
        landmark["y"]    = Json::Value(y);
        landmark["z"]    = Json::Value(z);
        landmark["nx"]    = Json::Value(nx);
        landmark["ny"]    = Json::Value(ny);
        landmark["nz"]    = Json::Value(nz);

        root.append(landmark);

      } // end for

      Json::StyledStreamWriter writer;
      writer.write(outFile, root);


    }
  private:
    // constructor should not be used
    LandmarkIO() {
    }
};

#endif
