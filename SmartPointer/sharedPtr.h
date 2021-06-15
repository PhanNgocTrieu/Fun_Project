#ifndef SHAREDPTR_H
#define SHAREDPTR_H
#include <iostream>

template <class _Tvar>
class mSharePtr
{
public:
    mSharePtr();
    mSharePtr(_Tvar* ptr);
    mSharePtr(const mSharePtr<_Tvar>& _other);

    ~mSharePtr();

    _Tvar* get();
    _Tvar& operator->();
    _Tvar& operator*();

    void reset();


protected:
    struct SharePtrCtrl
    {
        _Tvar *mPtrManage;
        int mSharePtrCount;
    };

    SharePtrCtrl *mSharePtrCtrl;

    mSharePtr& operator=(const mSharePtr& _other);
};

template <class _Tvar>
mSharePtr<_Tvar>::mSharePtr()
{
    printf("%d - %s \n",__LINE__, __FUNCTION__);
    mSharePtrCtrl = nullptr;
}

template<class _Tvar>
mSharePtr<_Tvar>::mSharePtr(_Tvar* ptr)
{
    
    mSharePtrCtrl = new SharePtrCtrl();
    mSharePtrCtrl->mPtrManage = ptr;
    mSharePtrCtrl->mSharePtrCount = 0;
    if (mSharePtrCtrl->mPtrManage)
    {
        mSharePtrCtrl->mSharePtrCount++;
    }
    printf("%d - %s \n",__LINE__, __FUNCTION__);
}

template<class _Tvar>
mSharePtr<_Tvar>::mSharePtr(const mSharePtr<_Tvar>& _other)
{
    std::cout << "Go here! Copy! " << std::endl;
    printf("%d - %s \n",__LINE__, __FUNCTION__);
    mSharePtrCtrl = new SharePtrCtrl;
    mSharePtrCtrl->mPtrManage = _other.mSharePtrCtrl->mPtrManage;
    mSharePtrCtrl->mSharePtrCount++;

}

template<class T>
mSharePtr<T>& mSharePtr<T>::operator=(const mSharePtr<T>& _other)
{
    std::cout << "Go here! Assignment" << std::endl;
    printf("%d - %s \n",__LINE__, __FUNCTION__);
    mSharePtrCtrl->mPtrManage = _other.mSharePtrCtrl->mPtrManage;
    mSharePtrCtrl->mSharePtrCount = _other.mSharePtrCtrl->mSharePtrCount;
    mSharePtrCtrl->mSharePtrCount++;
    
    return *this;
}

template<class T>
mSharePtr<T>::~mSharePtr()
{
    printf("%d - %s \n",__LINE__, __FUNCTION__);

    if (mSharePtrCtrl == nullptr)
        return;
    
    // count--
    mSharePtrCtrl->mSharePtrCount--;

    // checking for count == 0 -> delete
    if (mSharePtrCtrl->mSharePtrCount <= 0)
    {
        printf("%d - %s \n",__LINE__, __FUNCTION__);
        //delete prototype of mSharePtrControl
        delete mSharePtrCtrl->mPtrManage;

        //delete mSharePtrControl if all owners were already deleted 
        delete mSharePtrCtrl;
    }
}


template<class _Tvar>
_Tvar* mSharePtr<_Tvar>::get()
{
    if (mSharePtrCtrl == nullptr || mSharePtrCtrl->mSharePtrCount == 0)
        return nullptr;
    return mSharePtrCtrl->mPtrManage;
}

template<class _Tvar>
_Tvar& mSharePtr<_Tvar>::operator->()
{
    if (mSharePtrCtrl == nullptr || mSharePtrCtrl->mSharePtrCount == 0)
        throw std::runtime_error("Invalid Pointer");
    return *mSharePtrCtrl;
}

template<class _Tvar>
_Tvar& mSharePtr<_Tvar>::operator*()
{
    if (mSharePtrCtrl == nullptr || mSharePtrCtrl->mSharePtrCount == 0)
        throw std::runtime_error("Invalid Pointer");
    return &mSharePtrCtrl;
}


#endif
