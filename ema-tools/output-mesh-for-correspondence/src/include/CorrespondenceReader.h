#ifndef __CORRESPONDENCE_READER_H__
#define __CORRESPONDENCE_READER_H__

#include <vector>
#include <string>
#include <json/json.h>

#include "Correspondence.h"

class CorrespondenceReader{

public:

    static Correspondence read(const std::string& fileName) {
    
        Correspondence result;
    
        // try to open file
        std::ifstream inFile(fileName);
    
        // throw exception if file can not be opened
        if( inFile.is_open() == false) {
    
            throw std::runtime_error("Can not open correspondence file.");
    
        }
    
        // now parse the json file
        Json::Value root;

        inFile >> root;
        inFile.close();
    
        read_vertex_indices(root["vertexIndices"], result.vertexIndices);
        read_ema_channels(root["emaChannels"], result.emaChannels);
    
        return result;
    
    }

private:

    // hide the constructor -> we only have static methods
    CorrespondenceReader();

    static void read_vertex_indices( const Json::Value& data, std::vector<unsigned int>& result) {
    
        for( const Json::Value& index: data ) {
    
            result.push_back( index.asInt() );
    
        }
    
    }
    
    static void read_ema_channels( const Json::Value& data, std::vector<std::string>& result) {
    
        for( const Json::Value& entry: data ) {
    
            result.push_back( entry.asString() );
    
        }
    
    }

};
#endif
