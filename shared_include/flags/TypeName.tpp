#include <string>

#include "TypeName.h"

template<> std::string TypeName<int>::to_string()         { return "int";    }
template<> std::string TypeName<size_t>::to_string()      { return "int";    }
template<> std::string TypeName<float>::to_string()       { return "double"; }
template<> std::string TypeName<double>::to_string()      { return "double"; }
template<> std::string TypeName<std::string>::to_string() { return "string"; }
