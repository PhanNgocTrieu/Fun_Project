#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Point
{
public:
    Point();
    Point(const double &_x, const double &_y);
    Point(const Point &p);
    ~Point();

    void setX(const int &_x);
    void setY(const int &_y);
    double getX() const;
    double getY() const;

    friend istream &operator>>(istream &is, Point &p);
    friend ostream &operator>>(ostream &os, Point &p);

    Point operator+=(const Point &);
    Point operator-=(const Point &);
    Point operator*=(const double &__number);
    Point operator/=(const double &__number);

    

private:
    double x;
    double y;
};

class Shape
{
public:
    Shape(){};
    virtual ~Shape(){};
    virtual double getPerimeter() = 0;
    virtual double getArea() = 0;
    virtual Shape *fromString(const string &s) = 0;
    virtual string toString() = 0;
};

/*  ============================ FACTORY CLASS  ====================================  */
class Factory
{
public:
    Shape *createShape(int _type, const string &s);
    vector<Shape *> readShapesFromFile(const string &filename);
    void saveShapesToFile(const string &filename, const vector<Shape *> &shapes);
};

class Triangle : public Shape
{
public:
    Triangle() : A{0.0, 0.0}, B{0.0, 0.0}, C{0.0, 0.0} {}
    Triangle(const Point &p1, const Point &p2, const Point &p3) : A{p1}, B{p2}, C{p3} {}
    ~Triangle() {}
    void setA(const Point &p);
    void setB(const Point &p);
    void setC(const Point &p);
    Point getA();
    Point getB();
    Point getC();
    double getPerimeter();
    double getArea();
    Shape *fromString(const string &s);
    string toString();

private:
    Point A;
    Point B;
    Point C;
};
class Rectangle : public Shape
{
public:
    Rectangle() : A{0.0, 0.0}, B{0.0, 0.0}, C{0.0, 0.0}, D{0.0, 0.0} {}
    Rectangle(const Point &p1, const Point &p2, const Point &p3, const Point &p4) : A{p1}, B{p2}, C{p3}, D{p4} {}
    ~Rectangle() {}
    double getPerimeter() { return double(); }
    double getArea() { return double(); }
    Shape *fromString(const string &s);
    string toString() { return string(); }

private:
    Point A;
    Point B;
    Point C;
    Point D;
};
class Circle : public Shape
{
public:
    Circle() : I{0.0, 0.0}, R{0} {}
    Circle(const Point &p, const double r) : I{p}, R{r} {}
    ~Circle() {}
    double getPerimeter() { return double(); }
    double getArea() { return double(); }
    Shape *fromString(const string &s) { return this; }
    string toString() { return string(); }

private:
    Point I;
    double R;
};
class Ellipse : public Shape
{
public:
    Ellipse() : I{0.0, 0.0}, Rx{0}, Ry{0} {}
    Ellipse(const Point &p, const double &_x, const double &_y) : I{p}, Rx{_x}, Ry{_y} {}
    ~Ellipse() {}
    double getPerimeter() { return double(); }
    double getArea() { return double(); }
    Shape *fromString(const string &s) { return this; }
    string toString() { return string(); }

private:
    Point I;
    double Rx, Ry;
};

// class SimpleShapeFactory : public Factory
// {
// public:
//     Shape *createCurvedInstance()
//     {
//         return new Circle;
//     }
//     Shape *createStraightInstance()
//     {
//         return new Triangle;
//     }
// };
// class RobustShapeFactory : public Factory
// {
// public:
//     Shape *createCurvedInstance()
//     {
//         return new Ellipse;
//     }
//     Shape *createStraightInstance()
//     {
//         return new Rectangle;
//     }
// };

/*  ===================================  POINT  ======================================  */
// ********************** Constructor & Destructor ************************
Point::Point() : x{0}, y{0} {}
Point::Point(const double &_x, const double &_y) : x{_x}, y{_y} {}
Point::Point(const Point &p)
{
    x = p.x;
    y = p.y;
}
Point::~Point() {}
// ****************************** GET - SET *******************************
void Point::setX(const int &_x)
{
    x = _x;
}
void Point::setY(const int &_y)
{
    y = _y;
}
double Point::getX() const
{
    return x;
}
double Point::getY() const
{
    return y;
}
// ************************ Operator Overloading ***************************
Point Point::operator+=(const Point &p)
{
    return {x + p.x, y + p.y};
}
Point Point::operator-=(const Point &p)
{
    return Point(x - p.x, y - p.y);
}
Point Point::operator*=(const double &__number)
{
    return Point{x * __number, y * __number};
}
Point Point::operator/=(const double &__number)
{
    return Point{x / __number, y / __number};
}
// ************************ In - Out Overloading ***************************
istream &operator>>(istream &is, Point &p)
{
    cout << endl;
    cout << "Enter x: ", is >> p.x;
    cout << "Enter y: ", is >> p.y;
    return is;
}
ostream &operator>>(ostream &os, Point &p)
{
    os << "x: " << p.x << endl;
    os << "y: " << p.y << endl;
    return os;
}
Point operator+(const Point &p1, const Point &p2)
{
    Point temp = p1;
    return {temp += p2};
}
Point operator-(const Point &p1, const Point &p2)
{
    Point temp = p1;
    return {temp -= p2};
}
Point operator*(const Point &p, const double &__number)
{
    Point temp = p;
    temp *= __number;
    return Point(temp);
}
Point operator/(const Point &p, const double &__number)
{
    Point temp = p;
    temp /= __number;
    return Point(temp);
}
// ************************* Method Function ********************************
double distance(const Point &A, const Point &B)
{
    return sqrt(pow(B.getX() - A.getX(), 2) + pow(B.getY() - A.getY(), 2));
}
double dotProduct(const Point &A, const Point &B)
{
    // doi voi goc toa do)
    return (A.getX() * B.getX() + A.getY() * B.getY());
}
double dotProduct(const Point &A, const Point &B, const Point& C)
{
    return dotProduct(B-A,C-A);
}
double dotProduct(const Point &A, const Point &B, const Point& C, const Point& D)
{
    return dotProduct(B-A,D-C);
}
double theta(const Point& A, const Point& B, const Point& C, const Point& D)
{
    return double();
}



/*  ===================================  Shape  ======================================  */
//Shape::Shape() {}
//Shape::~Shape() {}
/*  ===================================  TRIANGLE  ======================================  */
void Triangle::setA(const Point &p)
{
    A = p;
}
void Triangle::setB(const Point &p)
{
    B = p;
}
void Triangle::setC(const Point &p)
{
    C = p;
}
Point Triangle::getA()
{
    return A;
}
Point Triangle::getB()
{
    return B;
}
Point Triangle::getC()
{
    return C;
}

double Triangle::getPerimeter()
{
    double side1 = distance(A, B);
    double side2 = distance(A, C);
    double side3 = distance(C, B);

    return side1 + side2 + side3;
}
double Triangle::getArea()
{
    double hafPer = getPerimeter() / 2;
    return sqrt(hafPer * (hafPer - distance(A, B)) * (hafPer - distance(A, C)) * (hafPer - distance(B, C)));
}
Shape *Triangle::fromString(const string &s)
{
    Shape *shape;
    if (s[0] != '0')
        return shape;
    int iRun = 0;
    char del = ' ';
    vector<int> words{};
    int push_text;
    stringstream read(s);
    Triangle triag;
    int i = 0;
    while (read >> push_text)
    {
        if (i != 0)
            words.push_back(push_text);
    }


    shape = &triag;
    return shape;
}
string Triangle::toString()
{
    return string();
}

/*  ===================================  FACTORY  ======================================  */

Shape *createShape(int _type, const string &s)
{
    Shape *create;
    if (_type == 0)
    {
        Triangle trg;
        trg.fromString(s);
        create = &trg;
    }
    // if (_type == 1)
    // {
    //     Rectangle rtg;
    //     rtg.fromString(s);
    //     create = &rtg;
    // }
    // if (_type == 2)
    // {
    //     Circle c;
    //     c.fromString(s);
    //     create = &c;
    // }
    // if (_type == 3)
    // {
    //     Ellipse ell;
    //     ell.fromString(s);
    //     create = &ell;
    // }

    return create;
}
vector<Shape *> readShapesFromFile(const string &filename)
{
    vector<Shape *> shapeList;
    int _type;
    fstream file;
    file.open(filename);
    string s;
    vector<string> save;
    while (getline(file, s))
    {
        save.push_back(s);
    }


    // processing string by string
    for (int idex = 0; idex < save.size(); idex++)
    {
        Shape* shape;
        string s = save[idex];
        if (s[0] == 0)
            shape = createShape(0,s);
        if (s[0] == 1)
            shape = createShape(1,s);
        if (s[0] == 2)
            shape = createShape(2,s);
        if (s[0] == 3)
            shape = createShape(3,s);
        
        shapeList.push_back(shape);
    }


    return shapeList;
}


int main()
{
    #ifdef SIMPLE
    Factory* factory = new SimpleShapeFactory;
    #elif ROBUST
    Factory* factory = new RobustShapeFactory;
    #endif


    Point p1(3, 5);
    cout << "test pow: " << pow(p1.getY(), p1.getX()) << endl;
    cout << "x = " << p1.getX() << ", y = " << p1.getY() << endl;
    cout << "x = " << p1.getX() << ", y = " << p1.getY() << endl;

    // vector<Shape *> shapeList;

    // string s = "0 3 4 5 3 22 11";

    // int iRun = 0;
    // char del = ' ';
    // vector<int> words{};
    // int push_text;
    // stringstream read(s);
    // Triangle triag;
    // while (read >> push_text)
    // {
    //     words.push_back(push_text);
    // }

    fstream File;
    //ofstream Log;
    File.open("triangle.txt");

    string s;
    getline(File,s);

    cout << s << endl;

    Triangle trg;
    Shape* shape;
    shape = trg.fromString(s);

    cout << shape->getArea();
    // std::ofstream Log("log_file.txt", std::ios_base::out | std::ios_base::app );
    //     Log << shape->toString() << endl;
    
    File.close();
    return 0;
}

// cái này mình có cần mấy cái file vs code không nhỷ? hay tạo 1 cái là nó tự động rồi?
