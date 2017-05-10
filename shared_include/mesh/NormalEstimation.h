#ifndef __NORMAL_ESTIMATION_H__
#define __NORMAL_ESTIMATION_H__

#include <vector>
#include <armadillo>

#include "Mesh.h"

// Based on Timo's code for computing normals,
// cf. Weights for Computing Vertex Normals from Facet Normals by Nelson Max
class NormalEstimation{

  public:

    /*-------------------------------------------------------------------------*/

    NormalEstimation(Mesh& mesh) :
      vertices(mesh.get_vertices()),
      faces(mesh.get_faces()) {

      init_vertex_handles();

    }

    /*-------------------------------------------------------------------------*/

    void set_mesh(Mesh& mesh) {
        this->vertices = mesh.get_vertices();
        this->faces = mesh.get_faces();
        init_vertex_handles();
    }

    /*-------------------------------------------------------------------------*/

    std::vector<arma::vec> compute() {

      std::vector<arma::vec> result;

      for(size_t i = 0; i < this->vertices.size(); ++i) {
        result.push_back(compute_normal(i));
      }

      return result;

    }

    /*-------------------------------------------------------------------------*/

  private:

    /*-------------------------------------------------------------------------*/

    // build mapping vertex -> faces
    void init_vertex_handles() {

      this->vertexHandles.clear();
      this->vertexHandles.resize(this->vertices.size());

      for(size_t i = 0; i < this->faces.size(); ++i) {

        // get vertex ids forming the face
        const auto faceVertices = faces.at(i);

        for(const unsigned int& index: faceVertices ) {
          this->vertexHandles[index].push_back(i);
        }
      }

      return;
    }

    /*-------------------------------------------------------------------------*/

    /* compute normal for vertex with the specified id */
    arma::vec compute_normal(const unsigned int& id) const {

      // get current vertex
      const arma::vec& current = this->vertices.at(id);
      arma::vec normal = arma::zeros(3);

      // process all faces this vertex belongs to
      for(const unsigned int& faceId: this->vertexHandles.at(id) ) {

        const auto& faceVertices = this->faces.at(faceId);

        // compute amount of participating vertices for this face
        const unsigned int size = faceVertices.size();

        // find position of vertex inside face
        for(size_t i = 0; i < faceVertices.size(); ++i) {

          if(faceVertices.at(i) == id) {

            // get indices of left and right neighbor of current vertex
            const unsigned int leftIndex  =
              faceVertices.at((i + size - 1) % size);
            const unsigned int rightIndex =
              faceVertices.at((i + 1    ) % size);

            // center both vertices at current vertex
            const arma::vec left =
              this->vertices.at(leftIndex) - current;
            const arma::vec right =
              this->vertices.at(rightIndex) - current;

            // compute facet normal
            const arma::vec facetNormal =
              arma::cross(right, left) /
              ( arma::dot(left, left) + arma::dot(right, right));

            normal += facetNormal;

          } // end if

        } // end for i

      } // end for faceId

      return arma::normalise(normal);

    }

    /*-------------------------------------------------------------------------*/

    std::vector<std::vector<unsigned int> > vertexHandles;

    std::vector<arma::vec>& vertices;
    std::vector<std::vector<unsigned int> >& faces;
};
#endif
