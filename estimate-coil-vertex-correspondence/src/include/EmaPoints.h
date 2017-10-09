#ifndef __EMA_POINTS_H__
  #define __EMA_POINTS_H__

  #include <json/json.h>
  #include <armadillo>
  #include <vector>
  #include <map>
  #include <string>

  class EmaPoints{

  public:

    void set_enforce_midsagittal(const bool& enforceMidsagittal) {

      this->enforceMidsagittal = enforceMidsagittal;

    }

    void set_shift(
                   const double& shiftX,
                   const double& shiftY,
                   const double& shiftZ
                   ) {

      this->shiftX = shiftX;
      this->shiftY = shiftY;
      this->shiftZ = shiftZ;

    }

    void set_scale(const double& scale) {

      this->scale = scale;

    }

    void set_channels(const std::vector<std::string> channels) {

      this->channels = channels;

    }

    std::vector<arma::vec> get_time_frame(int i) const {

      std::vector<arma::vec> timeFrame;

      for(const std::string& channel: this->channels) {
        timeFrame.push_back( this->channelData.at(channel).at(i) );
      }

      return timeFrame;

    }

    void add_points_from_file(const std::string& fileName) {

      std::ifstream inFile(fileName);

      // throw exception if file can not be opened
      if( inFile.is_open() == false ) {
        throw std::runtime_error("Cannot open json file.");
      }

      Json::Reader reader;
      reader.parse(inFile, this->json);
      inFile.close();

      for(const std::string& channel: this->channels) {

        add_points_from_channel(channel);

      }

    }

  private:

    void add_points_from_channel(const std::string& channelName) {

      // try to get channels
      const Json::Value channels = get_data("channels", this->json);

      // try to get channel
      const Json::Value channel = get_data(channelName, channels);

      // try to get position data
      const Json::Value position = get_data("position", channel);

      const unsigned int pointAmount = position.size() / 3;

      for( unsigned int i = 0; i < pointAmount; ++i) {

        Json::ArrayIndex index = i * 3;

        const double x =  this->scale * ( position[index + 0].asDouble() ) + this->shiftX;
        const double y =  this->scale * ( position[index + 1].asDouble() ) + this->shiftY;
        const double z = (this->enforceMidsagittal == true)? 0: this->scale * ( position[index + 2].asDouble() ) + this->shiftZ;

        const arma::vec point({x, y, z});

        this->channelData[channelName].push_back(point);

      } // end for

    }

    Json::Value get_data( const std::string& id, const Json::Value& value) {

      const Json::Value result = value[id];

      if(result.isNull()) {
        throw std::runtime_error("Can not access " + id + " data in json file.");
      }

      return result;

    }

    Json::Value json;
    double scale = 1;
    std::vector<std::string> channels;
    std::map< std::string, std::vector<arma::vec> > channelData;

    bool enforceMidsagittal = false;
    double shiftX = 0;
    double shiftY = 0;
    double shiftZ = 0;

  };

  #endif
