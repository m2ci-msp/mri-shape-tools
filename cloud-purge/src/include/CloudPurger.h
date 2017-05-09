#ifndef __CLOUD_PURGER_H__
#define __CLOUD_PURGER_H__

#include <vector>
#include "landmark/Landmark.h"
#include "mesh/Mesh.h"

class CloudPurger {

  public:

    CloudPurger(
        const Mesh& mesh,
        const std::vector<Landmark>& landmarks
        ) :
      mesh(mesh) {

      Landmark front;
      Landmark airway;

      for(const auto& mark: landmarks) {
        if( mark.name == "FrontBaseCenter") { this->front  = mark.targetPosition; } 
        if( mark.name == "Airway"         ) { this->airway = mark.targetPosition; } 
      }

      this->normal = compute_normal();


      }

    /*-------------------------------------------------------------------------*/

    Mesh purge() {
      const Mesh result = get_upper_part();

      return result;
    }

  private:

    /*-------------------------------------------------------------------------*/

    arma::vec compute_normal() const {

      const arma::vec z ({ 0, 0, 1});
      const arma::vec edge = this->front - this->airway;
      const arma::vec normal = arma::cross(z, edge);

      return arma::normalise(normal);

    }

    /*-------------------------------------------------------------------------*/

    Mesh get_upper_part() const {

      std::vector<arma::vec> oldVertices = this->mesh.get_vertices();
      std::vector<arma::vec> oldNormals  = this->mesh.get_vertex_normals();

      std::vector<arma::vec> newVertices;
      std::vector<arma::vec> newNormals;

      for(size_t i = 0; i < oldVertices.size(); ++i) {
        if( is_upper_part(oldVertices.at(i) ) == true  ) {
          newVertices.push_back(oldVertices.at(i));
          newNormals.push_back(oldNormals.at(i));
        }
      }

      Mesh result;
      result.set_vertices(newVertices)\
            .set_vertex_normals(newNormals);

      return result;

    }

    /*-------------------------------------------------------------------------*/

    bool is_upper_part( const arma::vec& point ) const {

      const arma::vec& direction = point - this->front;

      return (arma::norm_dot(direction, this->normal) >= 0);

    }

    /*-------------------------------------------------------------------------*/

    Mesh mesh;
    arma::vec front;
    arma::vec airway;
    arma::vec normal;
};
#endif
