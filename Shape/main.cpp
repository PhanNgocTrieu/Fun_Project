#include "Point.h"
#include <iostream>
using namespace std;


int main()
{
    Point p1(3,5);

    cout << "x = " << p1.getX() << ", y = " << p1.getY() << endl;
    return 0;
}