#ifndef __SCAN_IO_H__
#define __SCAN_IO_H__

#include <fstream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iomanip>

#include <json/json.h>

#include "scan/Scan.h"
#include "scan/ScanData.h"

class ScanIO{

  public:
    static Scan read_from(const std::string& fileName) {

      // try to open file
      std::ifstream inFile(fileName);

      // throw exception if file can not be opened
      if( inFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file.");
      }

      Json::Value root;
      Json::Reader reader;

      reader.parse(inFile, root);
      inFile.close();

      // read origin
      Json::Value origin = root["origin"];
      const double orgX = origin["x"].asDouble();
      const double orgY = origin["y"].asDouble();
      const double orgZ = origin["z"].asDouble();

      // read dimensions
      Json::Value dimensions = root["dimensions"];
      const int nx = dimensions["x"].asInt();
      const int ny = dimensions["y"].asInt();
      const int nz = dimensions["z"].asInt();

      // read spacings
      Json::Value spacings = root["spacings"];
      const double hx = spacings["x"].asDouble();
      const double hy = spacings["y"].asDouble();
      const double hz = spacings["z"].asDouble();

      // read extrema
      const double min = root["minimum"].asDouble();
      const double max = root["maximum"].asDouble();

      // read data
      std::vector<double> data;
      Json::Value values = root["data"];

      for( Json::Value& value: values) {
        data.push_back(value.asDouble());
      }

      // construct and return scan
      ScanData scanData;

      scanData.set_data(data)\
              .set_dimensions(nx, ny, nz)\
              .set_spacings(hx, hy, hz)\
              .set_extrema(min,max)\
              .set_origin(orgX, orgY, orgZ);

      return Scan(scanData);

    }

    /*-----------------------------------------------------------------------*/

    static void write_to(const std::string& fileName, const Scan& scan) {

      // try to open file
      std::ofstream outFile(fileName);

      // throw exception if file can not be opened
      if( outFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file.");
      }

      // create root node of json file
      Json::Value root(Json::objectValue);

      // add meta data

      // dimensions of the scan
      Json::Value dimensions(Json::objectValue);

      dimensions["x"] = scan.data()->get_nx();
      dimensions["y"] = scan.data()->get_ny();
      dimensions["z"] = scan.data()->get_nz();

      root["dimensions"] = dimensions;

      // origin of scan
      Json::Value origin(Json::objectValue);
      origin["x"] = scan.data()->get_org_x();
      origin["y"] = scan.data()->get_org_y();
      origin["z"] = scan.data()->get_org_z();

      root["origin"] = origin;

      // spacings along the grid axes
      Json::Value spacings(Json::objectValue);

      spacings["x"] = scan.data()->get_hx();
      spacings["y"] = scan.data()->get_hy();
      spacings["z"] = scan.data()->get_hz();

      root["spacings"] = spacings;

      // extemas
      root["minimum"] = scan.data()->get_min();
      root["maximum"] = scan.data()->get_max();

      // add data
      Json::Value data(Json::arrayValue);

      for( const auto& value: scan.data()->get_values() ) {
        data.append(value);
      }

      root["data"] = data;

      Json::StyledStreamWriter writer;
      writer.write(outFile, root);

      return;
    }

    /*-----------------------------------------------------------------------*/

    static void write_to_image_stack(
        const std::string& baseName,
        const Scan& data
        ) {

      Scan scan(*(data.data()));

      scan.transform()->scale_values(0, 255);

      const int nz = scan.data()->get_nz();

      for(int i = 0; i < nz; ++i) {
        write_image(scan, i, baseName);
      }


    }

    /*-----------------------------------------------------------------------*/

  private:

    static void write_image(
        const Scan& scan, const int& slice, const std::string& baseName) {

      std::ofstream pgmFile;

      const int nx = scan.data()->get_nx();
      const int ny = scan.data()->get_ny();

      // use string stream to build output name
      std::stringstream stream;
      stream << baseName;

      // compute number of digits for biggest number
      const int width = log(scan.data()->get_nz()) / log(10) + 1;

      // pad the numbers
      stream << std::setw(width) << std::setfill('0');
      stream << slice;

      // no padding of suffix
      stream << std::setw(4);
      stream << ".pgm";

      // open file
      pgmFile.open(stream.str());

      // write header
      pgmFile << "P5" << std::endl;
      pgmFile << nx << " ";
      pgmFile << ny << std::endl;
      pgmFile << 255 << std::endl;

      // output values in binary format
      for(int j = 0; j < ny; ++j) {
        for(int i = 0; i < nx; ++i) {
          const char value = (char) scan.access()->get_value_index(i, j, slice);
          pgmFile.put(value);
        }
      }

      pgmFile.close();

    }


    /*-----------------------------------------------------------------------*/

};
#endif
