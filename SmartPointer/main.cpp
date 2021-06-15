
#include <stdio.h>
#include <iostream>
#include <atomic>
#include <memory>

using namespace std;

template <class _Tvar>
class mSharePtr
{
public:
    mSharePtr();
    explicit mSharePtr(_Tvar *ptr);
    mSharePtr(const mSharePtr<_Tvar> &_other);
    ~mSharePtr();

    mSharePtr &operator=(const mSharePtr<_Tvar> &_other);
    mSharePtr &operator=(_Tvar *_other);

    _Tvar *get();
    _Tvar *operator->();
    _Tvar &operator*();
    bool operator==(const mSharePtr &_other) const;
    bool operator==(nullptr_t) const;
    bool operator!=(const mSharePtr &_other) const;
    bool operator!=(nullptr_t) const;

    int getRefCount() const;

    void reset();

    template <class __Tvar>
    friend ostream &operator<<(ostream &os, const mSharePtr<__Tvar> &obj);

private:
    void cleanUp();
    _Tvar *mPtr;
    int *ref_count;
};


template<class _Tvar>
atomic<int*> atmRefCount(mSharePtr<_Tvar>::ref_count);


/**
 *  @brief  Construct an empty %shared_ptr.
 *  @post   use_count()==0 && get()==0
 */
template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr()
    : mPtr{nullptr}, ref_count{new int(0)} //ref_count{nullptr}
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
}

/**
 *  @brief  Construct a %shared_ptr that owns the pointer @a __ptr.
 *  @param  @a_mPtr A pointer that is convertible to element_type.
 *  @post   use_count() == 1 && get() == __ptr
 *  @throw  std::bad_alloc, in which case @c delete @a __p is called.
 */
template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr(_Tvar *ptr) : mPtr{ptr}, ref_count {new int(0)}
{
    printf("Line: %d - %s(_Tvar * const ptr)\n", __LINE__, __FUNCTION__);
    if (mPtr)
        (*ref_count)++;
}


/**
 *  @brief  if @a_other is empty, constructs an empth %share_ptr
 *          otherwise construct a %share_ptr that shares ownership
 *          with @a_other
 *  @param  @a_mPtr A pointer that point to @a_other.
 *  @post   use_count() == _other.ref_count && get() == _other.mPtr (_other.get())
 *   
 */
template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr(const mSharePtr<_Tvar> &_other)
{
    printf("%d - %s(const mSharePtr<_Tvar>& _other)\n", __LINE__, __FUNCTION__);
    this->mPtr = _other.mPtr;
    this->ref_count = _other.ref_count; // reference to _other.ref_count
    if (this->mPtr)
    {
        (*this->ref_count)++;
    }
}

/**
 *  @brief  cleanUp _mPtr && ref_count.
 *          if _mPtr != nullptr 
 *              -> delete _mPtr;
 *              -> delte ref_count;
 *          otherwise ref_count--;
 *  @param  delete _mPtr;
 *          delete ref_count;
 *  @post   use_count() == nullptr && get() == nullptr
 *   
 */
template <class _Tvar>
mSharePtr<_Tvar>::~mSharePtr()
{
    cleanUp();
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
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

template <class _Tvar>
_Tvar *mSharePtr<_Tvar>::get()
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    if (mPtr == nullptr || (ref_count) == nullptr)
        return nullptr;
    return mPtr;
}

template <class _Tvar>
_Tvar *mSharePtr<_Tvar>::operator->()
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    if (mPtr == nullptr || (*ref_count) == 0)
        return nullptr;
    return mPtr;
}

template <class _Tvar>
_Tvar &mSharePtr<_Tvar>::operator*()
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    if (mPtr == nullptr || (*ref_count) == 0)
        throw std::runtime_error("Invalid Pointer");
    return *mPtr;
}

template <class _Tvar>
bool mSharePtr<_Tvar>::operator==(const mSharePtr &_other) const
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    return (this->mPtr == _other.mPtr && (*this->ref_count) == (*_other.ref_count));
}

template <class _Tvar>
bool mSharePtr<_Tvar>::operator==(nullptr_t) const
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    return (mPtr == nullptr && (ref_count) == nullptr);
}

template <class _Tvar>
bool mSharePtr<_Tvar>::operator!=(nullptr_t) const
{
    return (mPtr != nullptr || (ref_count) != nullptr);
}

template <class _Tvar>
bool mSharePtr<_Tvar>::operator!=(const mSharePtr &_other) const
{
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    return (mPtr != _other.mPtr || (*ref_count) != (*_other.ref_count));
}

template <class __Tvar>
ostream &operator<<(ostream &os, const mSharePtr<__Tvar> &obj)
{

    os << "Address: " << obj.mPtr << endl;
    os << "Value Counter: " << (*obj.ref_count) << endl;
    return os;
}

template <class _Tvar>
mSharePtr<_Tvar> &mSharePtr<_Tvar>::operator=(const mSharePtr<_Tvar> &_other)
{
    cleanUp();

    printf("%d - %s(_Tvar* ptr)\n", __LINE__, __FUNCTION__);
    this->mPtr = _other.mPtr;
    this->ref_count = _other.ref_count;
    if (mPtr)
    {
        (*this->ref_count)++;
    }

    return this;
}

template <class _Tvar>
mSharePtr<_Tvar> &mSharePtr<_Tvar>::operator=(_Tvar *_other)
{
    cleanUp();

    this->mPtr = _other;
    this->ref_count = new int(1);
    if (this->mPtr)
    {
        (*ref_count)++;
    }
}

template <class _Tvar>
void mSharePtr<_Tvar>::cleanUp()
{
    if (mPtr == nullptr)
        return;

    printf("(ref_count != 0)\n");
    (*ref_count)--;
    if ((*ref_count) <= 0)
    {
        printf("(ref_count == 0)\n");

        if (mPtr)
        {
            printf("(mPtr != nullptr)\n");
            delete mPtr;
            mPtr = nullptr;
        }

        delete ref_count;
        ref_count = nullptr;
    }
}

class Block
{
public:
    Block() { printf("Line: %d - %s\n", __LINE__, __FUNCTION__); }
    ~Block() { printf("Line: %d - %s\n", __LINE__, __FUNCTION__); }
};

int main()
{
    //int temp = 10;

    //std::unique_ptr<int> u2(&temp);
    //mSharePtr<int> p;
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // //std::cout << *u2 << std::endl;

    // case 1: Fine! Works well
    // mSharePtr<int> p1(new int(1231));
    // cout << p1 << endl;
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // {
    //     mSharePtr<int> p2(p1);
    //     cout << p1 << endl;
    //     printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // }

    // case 2: Error invalid pointer - how to solve this?
    int x = 10;
    mSharePtr<int> p(&x);
    cout << "p Ref_count: " << p.getRefCount() << endl;
    mSharePtr<int> p2(p);
    cout << "p Ref_count: " << p.getRefCount() << endl;
    cout << "p2 Ref_count: " << p2.getRefCount() << endl;
    printf("Line: %d - %s\n", __LINE__, __FUNCTION__);

    // case 3: Using class object
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // mSharePtr<Block> pBlock(new Block());
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // {
    //     mSharePtr<Block> pBlock2(pBlock);
    //     printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // }
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);

    return 0;
}
