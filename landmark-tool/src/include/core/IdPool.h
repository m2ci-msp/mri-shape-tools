#ifndef IDPOOL_H__
#define IDPOOL_H__

/** Template for a class that issues ids for objects of a given
 * type.
 *
 * The class uses the Singleton design pattern.
 */
template <typename T> class IdPool {
  public:
    /** Gets the unique instance of the object. The object is
     * created if it does not exist.
     */ 
    static IdPool<T> *get_instance();


    /** Gets an id for a pointer. 
     * @param[in] a The pointer.
     */
    int generate_new_id();

    /** Notify the pool that an object of its id type was destroyed and
      * that this id is no longer active.
      */
    void notify_of_destruction();

    /** Gets amount of active ids. */
    int get_amount_of_active_ids();
  private:
    /** Standard Constructor. This constructor is private to
     * prevent this class from having more than one instance.
     */
    IdPool();

    /** Deletes the current instance if present. */
    static void delete_instance(); 

    /** Amount of active ids. */
    int amountOfActiveIds;

    /** Current free id. */
    int currentFreeId;

    /** Pointer to single instance of the object. 
     * Note: This instance has to be set to 'nullptr' in
     * KeyPool.cpp to create an instance for a specific type T.
     */
    static IdPool<T> *instance;

};

// include the template class implementation
#include "IdPool.tpp"

#endif
