#ifndef __OBSERVATORY_H__
#define __OBSERVATORY_H__

#include <memory>

template <typename T> class Observatory {
  public:
    static void create() {
      if( observer == nullptr ) {
	observer = std::make_shared<T>();
      }
    }

    static void destroy() {
      observer = nullptr;
    }

  private:
    Observatory();
    static std::shared_ptr<T> observer;

};


#endif
