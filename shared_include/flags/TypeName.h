#ifndef __TYPE_NAME_H__
#define __TYPE_NAME_H__
#include <string>

/* utility class for translating type names to strings */
template<class T>
class TypeName
{
  public:
    static std::string to_string() { return T::typeName(); }
};

// include template implementation
#include "TypeName.tpp"

#endif
