#include "sharedPtr.h"
#include <stdio.h>
#include <iostream>

int main()
{
    int Int = 100;
    int *pInt = &Int;
    mSharePtr<int> p = pInt;
    printf("%d - %s \n", __LINE__, __FUNCTION__);
    {
        printf("%d - %s \n", __LINE__, __FUNCTION__);
        std::cout << "Address: " << p.get() << std::endl;
        mSharePtr<int> p1 = p;
        printf("%d - %s \n", __LINE__, __FUNCTION__);
        std::cout << "Address: " << p1.get() << std::endl;
    }

    printf("%d - %s \n", __LINE__, __FUNCTION__);
    return 0;
}
