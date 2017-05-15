#include "core/ObjectWithId.h"
#include "core/IdPool.h"

/*---------------------------------------------------------------------------*/

template<typename T>
ObjectWithId<T>::ObjectWithId() {
  this->id = IdPool<T>::get_instance()->generate_new_id();
}

/*---------------------------------------------------------------------------*/

template<typename T>
ObjectWithId<T>::~ObjectWithId() {
  IdPool<T>::get_instance()->notify_of_destruction();
}

/*---------------------------------------------------------------------------*/


template<typename T>
int ObjectWithId<T>::get_id() {
  return this->id;
}


