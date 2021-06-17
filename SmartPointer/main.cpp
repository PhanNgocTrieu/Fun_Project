
#include <stdio.h>
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include "sharedPtr.h"
//#include "weakPtr.h"
using namespace std;

class Block
{
public:
    Block() { printf("Line: %d - %s\n", __LINE__, __FUNCTION__); }
    ~Block() { printf("Line: %d - %s\n", __LINE__, __FUNCTION__); }
};

int main()
{
    //******************************** FOR SHARED POINTER **************************************************
    /**
 *  @brief: Case 1:
 *          Managing the address of normal type (int, float, double,...) when
 *          we allocated new space memory for them ( new int(...), new double(...), ...)
 *          Taking look what happen.
 *  @result:Work-well
 *          
 *  
*/
    // mSharePtr<int> p;
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // //case 1: Fine! Works well
    // mSharePtr<int> p1(new int(1231));
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // {
    //     mSharePtr<int> p2(p1);
    //     printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // }

    /**
 *  @brief: Case 2: initialize a variable call x has address @_x
 *          use share_ptr to manage @_x
 *          Taking look what happen
 *  @result:
 *          Error invalid pointer - how to solve this? 
 *          Output:
 *          Line: 67 - mSharePtr(_Tvar * const ptr)       
 *          (ref_count != 0)
 *          (ref_count != 0)
 *          (mPtr != nullptr)
 *          free(): invalid pointer
 *          Aborted
 *  @guess: 
 *          this happens because of x is a temporary variable in main scope
 *          so when we use share_ptr to manage address of this variable
 *          address of this variable would be delete after share_ptr would be destroyed
 *          
 *          but because x is a temporarily Integer variable
 *          so after going out of scope of main
 *          this would try to destroyed itself in the same way
 *          => This cause "Invalid Pointer" because we already deleted in share_ptr.
 * 
 * 
 *  @solve: we could not use memory space that has not been allocated to manage
 *          using pointer - which was allocate to manage it
 *          This would be well
*/
    // int x = 10;
    // mSharePtr<int> p(&x);
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);

    /**
 *  @brief: Case 3:
 *          Managing the address of user-define type (Block,...) when
 *          we allocated new space memory for them ( new Block(...), new Class-Type(...), new Struct-Type(...) )
 *          Taking look what happen.
 *  @result:Work-well
 *  
*/
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // mSharePtr<Block> pBlock(new Block());
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // {
    //     mSharePtr<Block> pBlock2(pBlock);
    //     printf("Line: %d - %s\n", __LINE__, __FUNCTION__);
    // }
    // printf("Line: %d - %s\n", __LINE__, __FUNCTION__);

    /**
 *  @brief: Case 4:
 *          Testing compararision methods of 2 share_ptr && themself with nullptr
 *          we initilize 2 share_ptr which manages 2 same address
 *          still initialize 1 share_ptr which manages another address
 *          comparing them with nullptr and each other
 *          Taking look what happen.
 *  @result:Work-well
 *  
*/
    // mSharePtr<Block> pBlock(new Block());
    // mSharePtr<Block> pBlock2(nullptr);
    // printf("Line: %d - %s()\n", __LINE__, __FUNCTION__);
    // if (pBlock == nullptr)
    //     cout << "pBlock 2 is nullptr! " << endl;

    // printf("Line: %d - %s()\n", __LINE__, __FUNCTION__);
    // if (pBlock != pBlock2)
    //     cout << "pBlock != pBlock2" << endl;
    // else
    //     cout << "pBlock == pBlock2" << endl;

    // pBlock2.reset();

    // if (pBlock2 == nullptr)
    //     cout << "already delete!" << endl;

    //******************************** FOR WEAK POINTER **************************************************

    {
        mWeakPtr<int> gw;
        mSharePtr<int> sharePointer(new int(42));
        gw = sharePointer;

    }
    // observe();
    return 0;
}