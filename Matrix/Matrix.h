
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
class Matrix
{
private:
    int rows;
    int cols;

    double** arr;
public:
    Matrix();
    Matrix(const int&, const int&);
    Matrix(double**,const int&, const int&);
    Matrix(const Matrix&);
    ~Matrix();
    friend std::istream& operator >>(std::istream&, Matrix&);
    friend std::ostream& operator <<(std::ostream&, Matrix&);
    
    friend Matrix operator/(const Matrix&, const Matrix&);
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
    Matrix& operator*=(const Matrix&);
    Matrix& operator*=(const double&);
    Matrix& operator/=(const double&);
    Matrix& operator=(const Matrix&);
 

    void allocSpace();
    void setRows(const int&);
    void setCols(const int&);
    int getRows() const;
    int getCols() const;
    double** getArr() const;
    void ipMatrix();
    void printMatrix();
    
    
    
};

 Matrix operator+(const Matrix&, const Matrix&);
 Matrix operator-(const Matrix&, const Matrix&);
 Matrix operator*(const Matrix&, const Matrix&);
 Matrix operator*(const Matrix&, const double&);
 Matrix operator/(const Matrix&, const double&);


 Matrix cofactor(const Matrix&, const int&, const int&);
 double determinant(const Matrix&);
 Matrix adjoint(const Matrix&);
 Matrix inverse(const Matrix&);

 Matrix Sum(const Matrix&, const Matrix&);
 Matrix Sub(const Matrix&, const Matrix&);
 Matrix MulMatrix(const Matrix&, const Matrix&);
 Matrix MulNumber(const Matrix&, const double&);
 Matrix DivMatrix(const Matrix&, const Matrix&);
 Matrix DivNumber(const Matrix&, const double&);
 void printConsole();
 void MultiConsole();
 void DivConsole();
 
#endif
