#include <iostream>
#include <atomic>
#include <stdio.h>
#include "sharedPtr.h"

template <class _Tvar>
class mSharePtr;

template <class _Tvar>
class mWeakPtr
{
public:
    mWeakPtr()
    {
        printf("Line: %d - %s() -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
        mPtrCtrl = nullptr;
    }
    mWeakPtr(const mSharePtr<_Tvar> &_sharedPtr)
    {
        printf("Line: %d - %s() -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
        mPtrCtrl = _sharedPtr.managedPtr;
        wRefCount = _sharedPtr.ref_count;
        if (mPtrCtrl)
            (*wRefCount)++;
    }
    ~mWeakPtr()
    {
        printf("Line: %d - %s() -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
        deallocated();
    }

    mSharePtr<_Tvar> lock()
    {
        printf("Line: %d - %s() -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
        return mSharePtr<_Tvar>(mPtrCtrl);
    }

    mWeakPtr& operator=(const mSharePtr<_Tvar>& _sharedPtr)
    {
        this->mPtrCtrl = _sharedPtr.managedPtr;
        this->wRefCount = _sharedPtr.ref_count;
        if (this->mPtrCtrl)
            (*this->wRefCount)++;
        printf("Line: %d - %s() -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
        return *this;
    }
    
    int getWeakRef() const
    {
        return (*this->wRefCount);
    }

private:
    void deallocated()
    {
        if (mPtrCtrl == nullptr)
        {
            return;
        }
        printf("Line: %d - %s(*wRefCount != 0) -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
        if ( (*wRefCount) > 0)
        {
            (*wRefCount)--;
        }
        if ( (*wRefCount) <= 0)
        {
            printf("Line: %d - %s(*wRefCount == 0) -- weakRef: %d\n", __LINE__, __FUNCTION__,getWeakRef());
            if (mPtrCtrl)
            {

                delete mPtrCtrl;
                mPtrCtrl = nullptr;
                delete wRefCount;
                wRefCount = nullptr;
            }
            
        }
    }
    std::atomic<_Tvar *> mPtrCtrl;
    std::atomic<int *> wRefCount;
};