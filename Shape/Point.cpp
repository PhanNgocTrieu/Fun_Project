#include "Point.h"
#include <iostream>

using namespace std;

// *************************CONSTRUCTOR - DESTRUCTOR *******************************
Point::Point()
{
    x = 0;
    y = 0;
}
Point::Point(const int& _x, const int& _y) : x {_x}, y{_y} {}
Point::Point(const Point& p)
{
    x = p.x;
    y = p.y;
}
Point::~Point()
{}

// ****************************** GET - SET *******************************
void Point::setX(const int& _x)
{
    x = _x;
}
void Point::setY(const int& _y)
{
    y = _y;
}
int Point::getX() const
{
    return x;
}
int Point::getY() const
{
    return y;
}



