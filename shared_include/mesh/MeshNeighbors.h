#ifndef __MESH_NEIGHBORS_H__
#define __MESH_NEIGHBORS_H__

#include <set>
#include <string>
#include <unordered_map>

class MeshNeighbors{

  /*-------------------------------------------------------------------------*/

public:

  /*-------------------------------------------------------------------------*/

  std::set<int> get_neighbors(const int& vertexId) const {

    return this->neighbors.at(vertexId);

  }

  /*-------------------------------------------------------------------------*/

  void add_neighbor(const int& vertexId, const int& neighborId ) {

    this->neighbors[vertexId].insert(neighborId);

  }

  /*-------------------------------------------------------------------------*/

  void add_description( const std::string& description ) {

    this->descriptions.insert(description);

  }

  /*-------------------------------------------------------------------------*/

  const std::set<std::string>& get_descriptions() const {
    return this->descriptions;
  }

  /*-------------------------------------------------------------------------*/

  const std::unordered_map<int, std::set<int> >& get_neighbor_map() const {
    return this->neighbors;
  }
  /*-------------------------------------------------------------------------*/

  MeshNeighbors& operator+(const MeshNeighbors& other) {

    for( const auto& mapEntry: other.neighbors ) {

      for( const int& neighbor: mapEntry.second ) {

        this->neighbors[mapEntry.first].insert(neighbor);

      }

    }

    for( const std::string& description: other.descriptions ) {
      this->descriptions.insert(description);
    }

    return *this;

  }

  /*-------------------------------------------------------------------------*/

  void clear() {

    this->neighbors.clear();
    this->descriptions.clear();

  }

  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  std::unordered_map< int, std::set<int> > neighbors;

  std::set<std::string> descriptions;

  /*-------------------------------------------------------------------------*/

};
#endif
