#ifndef OBJECTWITHID_H__
#define OBJECTWITHID_H__

/** Base class for objects that require to be equipped with
 * unique ids at creation time.
 */
template<typename T> class ObjectWithId {
  public:
    /** Standard Constructor. */
    ObjectWithId();

    /** Standard Destructor. */
    virtual ~ObjectWithId();

    /** Returns the id of the object. */
    int get_id();

  protected:
    /** The object's id */
    int id;
};

// include template implementation
#include "ObjectWithId.tpp"

#endif
