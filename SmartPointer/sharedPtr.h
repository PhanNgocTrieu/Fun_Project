#ifndef SHAREDPTR_H
#define SHAREDPTR_H
#include <stdio.h>
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include "weakPtr.h"


using namespace std;

template <class _Tvar>
class mSharePtr
{
public:
    //friend template<class _Tvar>
    friend class mWeakPtr<_Tvar>;

    mSharePtr();
    explicit mSharePtr(_Tvar *ptr);
    mSharePtr(const mSharePtr<_Tvar> &_other);
    ~mSharePtr();

    mSharePtr &operator=(const mSharePtr<_Tvar> &_other);
    mSharePtr &operator=(_Tvar *_other);

    _Tvar *get() const;
    _Tvar *operator->() const;
    _Tvar &operator*() const;
    bool operator==(const mSharePtr &_other) const;
    bool operator==(nullptr_t) const;
    bool operator!=(const mSharePtr &_other) const;
    bool operator!=(nullptr_t) const;
    explicit operator bool() const noexcept
    {
        return this->get() != nullptr;
    }

    int getRefCount() const;

    void swap(mSharePtr &_other) noexcept;
    void reset() noexcept;

    template <class __Tvar>
    friend ostream &operator<<(ostream &os, const mSharePtr<__Tvar> &obj);

private:
    void cleanUp();
    _Tvar *managedPtr;
    int *ref_count;
};

template <class _Tvar>
atomic<int *> atmRefCount(mSharePtr<_Tvar>::ref_count);

template <class _Tvar>
atomic<_Tvar *> atmSharePtr(mSharePtr<_Tvar>::managedPtr);

/**
 *  @brief Construct an empty %shared_ptr.
 *  @param getRefCount()==0 && get()==nullptr
 */
template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr() //ref_count{nullptr}
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    managedPtr = nullptr;
    ref_count = new int(0);
}

/**
 *  @brief  Construct a %shared_ptr that owns the pointer __ptr
 *  @param  @a _mPtr A pointer that is convertible to element_type
 *  @post   getRefcount() == 1 && get() == _ptr
 *  @throw  std::bad_alloc, in which case @c delete @a __p is called.
 */
template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr(_Tvar *ptr) : managedPtr{ptr}, ref_count{new int(0)}
{
    printf("Line: %d - %s(_Tvar * const ptr)\n", __LINE__, __FUNCTION__);
    if (managedPtr)
        (*ref_count)++;
}

/**
 *  @brief  if @a _other is empty, constructs an empth %share_ptr
 *          otherwise construct a %share_ptr that shares ownership
 *          with @a _other
 *  @param  @a _mPtr A pointer that point to @a _other.
 *  @post   _get() == _other.ref_count && _ get() == _other.managedPtr (_other.get())
 *   
 */
template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr(const mSharePtr<_Tvar> &_other)
{
    printf("%d - %s(const mSharePtr<_Tvar>& _other)\n", __LINE__, __FUNCTION__);
    this->managedPtr = _other.managedPtr;
    this->ref_count = _other.ref_count; // reference to _other.ref_count
    if (this->managedPtr)
    {
        (*this->ref_count)++;
    }
}

/**
 *  @brief cleanUp _mPtr && _ref_count - rely on reference_counter - whether it's 0 or not
 *  @param deallocated _mPtr && _ref_count
 *  @post  delete _mPtr; delete ref_count  
 */
template <class _Tvar>
mSharePtr<_Tvar>::~mSharePtr()
{
    printf("Line: %d - %s()\n", __LINE__, __FUNCTION__);
    cleanUp();
}

/**
 *  @brief  get value of Reference Counter
 *  @param  getRefCount = (*this->ref_count)
 *  @post   use_count() == (*this->ref_count)
 *   
 */
template <class _Tvar>
int mSharePtr<_Tvar>::getRefCount() const
{
    //printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    return (*ref_count);
}

/**
 *  @brief  get value of pointer which is managing
 *  @param  return value of managedPtr
 *  @post   get() == *this->managedPtr
 *   
 */
template <class _Tvar>
_Tvar *mSharePtr<_Tvar>::get() const
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    return managedPtr;
}

/**
 *  @brief  operator-> overloading
 *          return type must be a pointer or an object of a class to which you can apply.
 *  @param  get() { return managedPtr; }
 *  @post   get() == this->managedPtr
 *   
 */
template <class _Tvar>
_Tvar *mSharePtr<_Tvar>::operator->() const
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    if (managedPtr == nullptr)
        return nullptr;
    return managedPtr;
}

/**
 *  @brief  operator-> overloading
 *          dereference 
 *  @param  get() { return *managedPtr; }
 *  @post   get() == *this->managedPtr
 *   
 */
template <class _Tvar>
_Tvar &mSharePtr<_Tvar>::operator*() const
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    if (managedPtr == nullptr)
        throw std::runtime_error("Invalid Pointer");
    return *managedPtr;
}

/**
 *  @brief  operator== overloading.
 *          it would be true if _mPtr == _other->_mPtr     
 *  @param  _other sharePtr object
 *  @param  _mPtr *this->_mPtr
 *  @post   get() == *this->managedPtr
 *   
 */
template <class _Tvar>
bool mSharePtr<_Tvar>::operator==(const mSharePtr &_other) const
{
    printf("Line: %d - %s(const mSharePtr& _other)\n", __LINE__, __FUNCTION__);
    return (this->managedPtr == _other.managedPtr);
}

/**
 *  @brief  operator== overloading.
 * it would be true if _mPtr == nullptr
 *           
 *  @param  nullptr_t nullptr_type,
 *  @param  _mPtr Ptr_manage
 *   
 */
template <class _Tvar>
bool mSharePtr<_Tvar>::operator==(nullptr_t) const
{
    printf("Line: %d - %s(nullptr_t)\n", __LINE__, __FUNCTION__);
    return (managedPtr == nullptr);
}

/**
 *  @brief  operator!= overloading.
 * it would be true if _mPtr != nullptr
 *           
 *  @param  nullptr_t nullptr_type,
 *  @param  _mPtr Ptr_manage
 *  
 */
template <class _Tvar>
bool mSharePtr<_Tvar>::operator!=(nullptr_t) const
{
    printf("Line: %d - %s(nullptr_t)\n", __LINE__, __FUNCTION__);
    return (managedPtr != nullptr);
}

/**
 *  @brief  operator!= overloading.
 * it would be true if _mPtr != _other
 *           
 *  @param  _ohter __sharePtr_object,
 *  @param  _mPtr internal __sharePtr_object
 *  
 */
template <class _Tvar>
bool mSharePtr<_Tvar>::operator!=(const mSharePtr &_other) const
{
    printf("Line: %d - %s(const mSharePtr& _other)\n", __LINE__, __FUNCTION__);
    return (managedPtr != _other.managedPtr);
}

template <class __Tvar>
ostream &operator<<(ostream &os, const mSharePtr<__Tvar> &obj)
{
    os << "Address: " << obj.managedPtr << endl;
    os << "Value Counter: " << (*obj.ref_count) << endl;
    return os;
}

template <class _Tvar>
mSharePtr<_Tvar> &mSharePtr<_Tvar>::operator=(const mSharePtr<_Tvar> &_other)
{
    cleanUp();

    printf("%d - %s(_Tvar* ptr)\n", __LINE__, __FUNCTION__);
    this->managedPtr = _other.managedPtr;
    this->ref_count = _other.ref_count;
    if (managedPtr)
    {
        (*this->ref_count)++;
    }

    return this;
}

template <class _Tvar>
mSharePtr<_Tvar> &mSharePtr<_Tvar>::operator=(_Tvar *_other)
{
    cleanUp();

    this->managedPtr = _other;
    this->ref_count = new int(1);
    if (this->managedPtr)
    {
        (*ref_count)++;
    }
}

/**
 *  @brief  Deallocated memory space that we already allocated
 *          Trying to deallocated all of share_ptr participate in ownership of the same source
 *          Share_ptr Object would be delete when reference_counter would be 0.
 *          Taking look what happen.
 *  @param  _mPtr internal _sharePtr_Object
 *  @param  ref_count reference counter
 *  @result deallocated objects who participate in ownership of the same source. _mPtr == allocated if reference_counter == 0
 *          
 *  
*/
template <class _Tvar>
void mSharePtr<_Tvar>::cleanUp()
{
    if (managedPtr == nullptr)
        return;

    printf("(ref_count != 0)\n");
    (*ref_count)--;
    if ((*ref_count) <= 0)
    {
        printf("(ref_count == 0)\n");

        if (managedPtr)
        {
            printf("(managedPtr != nullptr)\n");
            delete managedPtr;
            managedPtr = nullptr;
        }
        delete ref_count;
        ref_count = nullptr;
    }
}

template <class _Tvar>
void mSharePtr<_Tvar>::swap(mSharePtr &_other) noexcept
{
    this->managedPtr = move(_other.managedPtr);
    this->ref_count = move(_other.ref_count);

    _other.~mSharePtr();
}

template <class _Tvar>
void mSharePtr<_Tvar>::reset() noexcept
{ // release resource and convert to empty shared_ptr object
    mSharePtr().swap(*this);
}

#endif