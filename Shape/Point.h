#ifndef __POINT_H__
#define __POINT_H__

class Point
{
    public:
        Point();
        Point(const int& _x, const int& _y);
        Point(const Point& p);
        ~Point();
        
        void setX(const int& _x);
        void setY(const int& _y);
        int getX() const;
        int getY() const;



    private:
        int x;
        int y;
};



#endif