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
    Factory() {}
    ~Factory() {}
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
    Point getA() const;
    Point getB() const;
    Point getC() const;
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
    Rectangle() : I{0.0, 0.0}, mwidth{0}, mlength{0} {}
    Rectangle(const Point &p1, const double &_Width, const double &_Length) : I{p1}, mwidth{_Width}, mlength{_Length} {}
    ~Rectangle() {}
    double getPerimeter();
    double getArea();
    Shape *fromString(const string &s);
    string toString();
    void setI(const Point &_A);
    void setWidth(const double &_B);
    void setLength(const double &_C);

    Point getI() const;
    double getWidth() const;
    double getLength() const;

private:
    Point I;
    double mwidth;
    double mlength;
};
class Circle : public Shape
{
public:
    Circle() : I{0.0, 0.0}, R{0} {}
    Circle(const Point &p, const double r) : I{p}, R{r} {}
    ~Circle() {}
    double getPerimeter();
    double getArea();
    Shape *fromString(const string &s);
    string toString();
    void setI(const Point &_I);
    void setR(const double &_R);
    Point getI() const;
    double getR() const;

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
    double getPerimeter();
    double getArea();
    Shape *fromString(const string &s);
    string toString();
    void setI(const Point &_I);
    void setRx(const double &_Rx);
    void setRy(const double &_Ry);
    Point getI() const;
    double getRx() const;
    double getRy() const;

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
double dotProduct(const Point &A, const Point &B, const Point &C)
{
    return dotProduct(B - A, C - A);
}
double dotProduct(const Point &A, const Point &B, const Point &C, const Point &D)
{
    return dotProduct(B - A, D - C);
}



/*  ===================================  Shape  =========================================  */

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
Point Triangle::getA() const
{
    return A;
}
Point Triangle::getB() const
{
    return B;
}
Point Triangle::getC() const
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
    if (s[0] != '0')
        return (this);
    int iRun = 0;
    vector<double> words{};
    int push_text;
    stringstream read(s);
    //Triangle triag;
    int i = 0;
    while (read >> push_text)
    {
        if (i != 0)
        {
            words.push_back(push_text);
        }
        else
        {
            //
        }
        i++;
    }
    //cout << words[0] << endl;
    Point A{words[0], words[1]};
    Point B{words[2], words[3]};
    Point C{words[4], words[5]};
    setA(A);
    setB(B);
    setC(C);

    return (this);
}
string Triangle::toString()
{
    string s = "";
    s += to_string(0) + " ";
    s += to_string(static_cast<int>(getA().getX())) + " ";   // Ax
    s += to_string(static_cast<int>(getA().getY())) + " ";   // Ay
    s += to_string(static_cast<int>(getB().getX())) + " ";   // Bx
    s += to_string(static_cast<int>(getB().getY())) + " ";   // By
    s += to_string(static_cast<int>(getC().getX())) + " ";   // Cx
    s += to_string(static_cast<int>(getC().getY())) + " ";   // Cy
    return string(s);
}

/* *************************** RECTANGLE ****************************/
/* ***** GET - SET ****************/
void Rectangle::setI(const Point &_I)
{
    I = _I;
}
void Rectangle::setWidth(const double &_Width)
{
    mwidth = _Width;
}
void Rectangle::setLength(const double &_Length)
{
    mlength = _Length;
}
Point Rectangle::getI() const
{
    return I;
}
double Rectangle::getLength() const
{
    return mlength;
}
double Rectangle::getWidth() const
{
    return mwidth;
}
double Rectangle::getPerimeter()
{
    return (mwidth + mlength) * 2;
}
double Rectangle::getArea()
{
    return mlength * mwidth;
}
Shape *Rectangle::fromString(const string &s)
{
    if (s[0] != '1')
        return (this);
    int iRun = 0;
    vector<double> words{};
    int push_text;
    stringstream read(s);
    //Triangle triag;
    int i = 0;
    while (read >> push_text)
    {
        if (i != 0)
        {
            words.push_back(push_text);
        }
        i++;
    }
    //cout << words[0] << endl;
    Point I{words[0], words[1]};
    mlength = words[2];
    mwidth = words[3];
    setI(I);

    return this;
}
string Rectangle::toString()
{
    string s = "";
    s += to_string(1) + " ";
    s += to_string(static_cast<int>(getI().getX())) + " ";   // Ix
    s += to_string(static_cast<int>(getI().getY())) + " ";   // Iy
    s += to_string(static_cast<int>(getWidth())) + " ";      // Width
    s += to_string(static_cast<int>(getLength())) + " ";     // Length
    return string(s);
}

/* *************************** Circle ****************************/
void Circle::setI(const Point &_I)
{
    I = _I;
}
void Circle::setR(const double &_R)
{
    R = _R;
}
Point Circle::getI() const
{
    return I;
}
double Circle::getR() const
{
    return R;
}
double Circle::getPerimeter()
{
    return double(R * 2 * 3.14);
}
double Circle::getArea()
{
    return R * R * 3.14;
}
Shape *Circle::fromString(const string &s)
{
    if (s[0] != '2')
        return (this);
    int iRun = 0;
    vector<double> words{};
    int push_text;
    stringstream read(s);
    //Triangle triag;
    int i = 0;
    while (read >> push_text)
    {
        if (i != 0)
        {
            words.push_back(push_text);
        }
        i++;
    }
    //cout << words[0] << endl;
    Point I{words[0], words[1]};
    setI(I);
    R = words[2];

    return this;
}
string Circle::toString()
{
    string s = "";
    s += to_string(2) + " ";
    s += to_string(static_cast<int>(getI().getX())) + " ";   // Ix
    s += to_string(static_cast<int>(getI().getY())) + " ";   // Iy
    s += to_string(static_cast<int>(getR())) + " ";         // R
    return string(s);
}

/* *************************** Ellipse ****************************/
void Ellipse::setI(const Point &_I)
{
    I = _I;
}
void Ellipse::setRx(const double &_Rx)
{
    Rx = _Rx;
}
void Ellipse::setRy(const double &_Ry)
{
    Ry = _Ry;
}
Point Ellipse::getI() const
{
    return I;
}
double Ellipse::getRx() const
{
    return Rx;
}
double Ellipse::getRy() const
{
    return Ry;
}
double Ellipse::getPerimeter()
{
    if (Rx == Ry)
        return 2 * Rx * 3.14159;
    else
    {
        /*
        h = (a-b)^2 / (a+b)^2;
        p ~= PI * (a + b) * (1 + 3*h/(10+ sqrt(4 - 3*h)) );
        */
        double h = pow((Rx - Ry), 2) / pow((Rx + Ry), 2);
        double x = 3.1415 * (Rx + Ry) * (1 + 3 * h / (10 + sqrt(4 - 3 * h)));
        return x;
    }
}
double Ellipse::getArea()
{
    return 3.14159 * Rx * Ry;
}
Shape *Ellipse::fromString(const string &s)
{
    if (s[0] != '3')
        return (this);
    int iRun = 0;
    vector<double> words{};
    int push_text;
    stringstream read(s);
    while (read >> push_text)
    {
        if (iRun != 0)
        {
            words.push_back(push_text);
        }
        iRun++;
    }
    Point I{words[0], words[1]};
    setI(I);
    Rx = words[2];
    Ry = words[3];
    return this;
}
string Ellipse::toString()
{
    // writing shape to string
    string s = "";
    s += to_string(3) + " ";
    s += to_string(static_cast<int>(getI().getX())) + " ";   // Ix
    s += to_string(static_cast<int>(getI().getY())) + " ";   // Iy
    s += to_string(static_cast<int>(getRx())) + " ";         // Rx
    s += to_string(static_cast<int>(getRy())) + " ";         // Ry
    return string(s);
}

/*  ===================================  FACTORY  ======================================  */

Shape *Factory::createShape(int _type, const string &s)
{
    Shape *create;
    if (_type == 0)
    {
        create = new Triangle;
        create->fromString(s);
        return create;
    }
    if (_type == 1)
    {
        create = new Rectangle;
        create->fromString(s);
        return create;
    }
    if (_type == 2)
    {
        create = new Circle;
        create->fromString(s);
        return create;
    }
    if (_type == 3)
    {
        create = new Ellipse;
        create->fromString(s);
        return create;
    }
    return create;
}
vector<Shape *> Factory::readShapesFromFile(const string &filename)
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
        string s = save[idex];
        if (s[0] == '0')
        {
            cout << "Make Triangle!" << endl;
            shapeList.push_back(createShape(0, s));
        }
        if (s[0] == '1')
        {
            cout << "Make Rectangle!" << endl;
            shapeList.push_back(createShape(1, s));
        }
        if (s[0] == '2')
        {
            cout << "Make Circle!" << endl;
            shapeList.push_back(createShape(2, s));
        }
        if (s[0] == '3')
        {
            cout << "Make Ellipse!" << endl;
            shapeList.push_back(createShape(3, s));
        }
    }
    file.close();
    return shapeList;
}
void Factory::saveShapesToFile(const string &filename, const vector<Shape *> &shapes)
{
    std::ofstream Log("log_file.txt", std::ios_base::out | std::ios_base::trunc);
    for (int idex = 0; idex < shapes.size(); idex++)
    {
        Log << "Input: " << shapes[idex]->toString() << endl;
        Log << "\tOutput:" << shapes[idex]->getArea()  << "\t" << shapes[idex]->getPerimeter() << endl;
    }
}


int main()
{
    Factory *fac = new Factory;
    string filename = "data.txt";
    string fileLog = "log_file.txt";
    vector<Shape *> shapeList;
    shapeList = fac->readShapesFromFile(filename);
    fac->saveShapesToFile(fileLog, shapeList);

    delete fac;
    return 0;
}
