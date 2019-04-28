#ifndef __OUTPUT_RESULT_H__
#define __OUTPUT_RESULT_H__

#include <fstream>
#include <vector>
#include <json/json.h>

class OutputResult{

public:

    OutputResult& set_partition_index(const int& partitionIndex) {
    
      this->partitionIndex = partitionIndex;
      this->partitionIndexSet = true;
    
      return *this;
    
    }
    
    OutputResult& set_partition_amount(const int& partitionAmount) {
    
      this->partitionAmount = partitionAmount;
      this->partitionAmountSet = true;
    
      return *this;
    
    }
    
    OutputResult& set_energy(const double& energy) {
    
      this->energy = energy;
      this->energySet = true;
    
      return *this;
    
    }
    
    OutputResult& set_vertex_indices(const std::vector<unsigned int>& vertexIndices) {
    
      this->vertexIndices = vertexIndices;
      this->vertexIndicesSet = true;
    
      return *this;
    
    }
    
    OutputResult& set_ema_channels(const std::vector<std::string>& emaChannels) {
    
      this->emaChannels = emaChannels;
      this->emaChannelsSet = true;
    
      return *this;
    
    }
    

    void write(const std::string& fileName) const {
    
        // try to open file
        std::ofstream outFile(fileName);
    
        // create root node of json file
        Json::Value root(Json::objectValue);
    
        root["partitionIndex"] = Json::Value(this->partitionIndex);
        root["partitionAmount"] = Json::Value(this->partitionAmount);
        root["energy"] = Json::Value(this->energy);
    
        // vertex indices
        Json::Value indices(Json::arrayValue);
    
        for(const unsigned int& index: this->vertexIndices) {
    
            indices.append(Json::Value(index));
    
        }
    
        root["vertexIndices"] = indices;
    
        // ema channels
        Json::Value channels(Json::arrayValue);
    
        for(const std::string& channel: this->emaChannels) {
    
            channels.append(Json::Value(channel));
    
        }
    
        root["emaChannels"] = channels;
    
        outFile << root << std::endl;
    
        outFile.close();
    
    }

private:

    bool verify() const {
    
        return (
                this->partitionIndexSet &&
                this->partitionAmountSet &&
                this->energySet &&
                this->vertexIndicesSet &&
                this->emaChannelsSet
                );
    
      }

    int partitionIndex;
    int partitionAmount;
    double energy;
    std::vector<unsigned int> vertexIndices;
    std::vector<std::string> emaChannels;
    
    bool partitionIndexSet = false;
    bool partitionAmountSet = false;
    bool energySet = false;
    bool vertexIndicesSet = false;
    bool emaChannelsSet = false;

};
#endif
