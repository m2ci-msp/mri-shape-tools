#ifndef __SEARCH_PROTO_H__
#define __SEARCH_PROTO_H__

#include <vector>

/* abstract class describing interface for search strategies */
class SearchProto{

public:

  /*-------------------------------------------------------------------------*/

  virtual ~SearchProto() {
  }

  /*-------------------------------------------------------------------------*/

  virtual void find_neighbors(
    std::vector<int>& sourceIndices,
    std::vector<int>& targetIndices
    ) const  = 0;

  /*-------------------------------------------------------------------------*/

};
#endif
