#include "core/IdPool.h"

/*---------------------------------------------------------------------------*/

template<typename T>
IdPool<T>::IdPool() :  amountOfActiveIds(0), currentFreeId(1) {
}

/*---------------------------------------------------------------------------*/

template<typename T>
  IdPool<T> *IdPool<T>::get_instance() {
    if( ! instance ) {
      instance = new IdPool<T>();
    }

    return instance;
  }

/*---------------------------------------------------------------------------*/

template<typename T>
  int IdPool<T>::generate_new_id() {

    // increase amount of active ids
    ++this->amountOfActiveIds;
    
    // return current free id and increment it afterwards
    return this->currentFreeId++;
  }

/*---------------------------------------------------------------------------*/

template<typename T>
  void IdPool<T>::notify_of_destruction() {
    // decrease amount of active ids
    --this->amountOfActiveIds;

    // free memory if no ids are active
    if( this->amountOfActiveIds <= 0 ) {
      IdPool<T>::delete_instance();
    }
  }
/*---------------------------------------------------------------------------*/

template<typename T>
  int IdPool<T>::get_amount_of_active_ids() {
    return this->amountOfActiveIds;
  }

/*---------------------------------------------------------------------------*/

template<typename T>
  void IdPool<T>::delete_instance() {
    if( instance ) {
      delete instance;
      instance = nullptr;
    }
  }
