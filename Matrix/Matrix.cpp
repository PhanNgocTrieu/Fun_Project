#include "Matrix.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <iomanip>
using namespace std;


// *************************** Constructor ********************************
Matrix::Matrix(const int& row, const int& col) : rows{ row }, cols{ col }
{
    allocSpace();
    for (int iRows = 0; iRows < rows; ++iRows)
    {
        for (int iCols = 0; iCols < cols; ++iCols)
        {
            arr[iRows][iCols] = 0;
        }
    }
}
Matrix::Matrix()
{
    rows = 1;
    cols = 1;
    allocSpace();
    arr[0][0] = 0;
}
Matrix::Matrix(double** ar, const int& row, const int& col)
{
    allocSpace();
    for (int iRows = 0; iRows < rows; ++iRows) {
        for (int iCols = 0; iCols < cols; ++iCols)
        {
            arr[iRows][iCols] = ar[iRows][iCols];
        }
    }
}
Matrix::Matrix(const Matrix& max) : rows {max.rows}, cols {max.cols}
{
    allocSpace();
    for (int iRows = 0; iRows < rows; ++iRows) {
        for (int iCols = 0; iCols < cols; ++iCols)
        {
            arr[iRows][iCols] = max.arr[iRows][iCols];
        }
    }
}


// *************************** Destructor ********************************
Matrix::~Matrix()
{
    for (int iRows = 0; iRows < rows; iRows++)
    {
        delete[] arr[iRows];
        arr[iRows] = NULL;
    }
    delete arr;
    arr = NULL;
}


// *************************** Method ************************************
void Matrix::setRows(const int& row)
{
    rows = row;
}
void Matrix::setCols(const int& col)
{
    cols = col;
}
int Matrix::getRows()const
{
    return rows;
}
int Matrix::getCols()const
{
    return cols;
}
double** Matrix::getArr() const
{
    return arr;
}
void Matrix::allocSpace()
{
    arr = new double* [rows];
    for (int i = 0; i < rows; ++i) 
    {
        arr[i] = new double[cols];
    }
}
void Matrix::ipMatrix()
{
    
    cout << "Enter rows: ", cin >> rows;
    cout << "Enter cols: ", cin >> cols;
    while (rows != cols)
    {
        cout << "Enter again! Make sure NxN matrix!" << endl;
        cout << "Enter rows: ", cin >> rows;
        cout << "Enter cols: ", cin >> cols;
    }
    
    allocSpace();
    cin >> *this;
}
void Matrix::printMatrix()
{
    cout << *this;
}

// *************************** Overloading ********************************
istream& operator >> (istream& is, Matrix& mat)
{
    cout << endl;
    for (int iRows = 0; iRows < mat.rows; ++iRows) {
        for (int iCols = 0; iCols < mat.cols; ++iCols)
        {
            cout << "Enter Matrix[" << iRows << "][" << iCols << "]: ";
            is >> mat.arr[iRows][iCols];
        }
    }
    return is;
}
std::ostream& operator <<(std::ostream& os, Matrix& mat)
{
    cout << endl;
    for (int iRows = 0; iRows < mat.rows; ++iRows) {
        for (int iCols = 0; iCols < mat.cols; ++iCols)
        {
            os << mat.arr[iRows][iCols] << "  ";
        }
        os << endl;
    }
    return os;
}

// Deal with 2 same level matrixs
Matrix operator+(const Matrix& mat1, const Matrix& mat2)
{
    Matrix temp(mat1);
    return (temp += mat2);

}
Matrix operator-(const Matrix& mat1, const Matrix& mat2)
{
    Matrix t_mat{ mat1 };
    return (t_mat -= mat2);
}
Matrix operator*(const Matrix& mat1, const Matrix& mat2)
{
    Matrix temp{ mat1 };
    return (temp *= mat2);
}
Matrix operator*(const Matrix& mat1, const double& num)
{
    Matrix tMat{ mat1 };
    return (tMat *= num);
}
Matrix operator/(const Matrix& mat, const double& num)
{
    Matrix tMat{ mat };
    return (tMat /= num);
}
Matrix operator/(const Matrix& mat1, const Matrix& mat2)
{
    assert(mat1.getCols() == mat2.getCols());
    Matrix ivMat2 = inverse(mat2);

    cout << "\n Print inside division 2 matrixs - inverse matrix: " << endl;
    ivMat2.printMatrix();
    
    Matrix temp{ mat1 };
    return (temp *= ivMat2);
}
Matrix& Matrix::operator+=(const Matrix& mat)
{
    for (int iRows = 0; iRows < rows; iRows++)
    {
        for (int iCols = 0; iCols < cols; iCols++)
        {
            arr[iRows][iCols] += mat.arr[iRows][iCols];
        }
    }
    return *this;
}
Matrix& Matrix::operator-=(const Matrix& mat)
{
    for (int iRows = 0; iRows < rows; iRows++)
    {
        for (int iCols = 0; iCols < cols; iCols++)
        {
             arr[iRows][iCols] -= mat.arr[iRows][iCols];
        }
    }
    return *this;
}
Matrix& Matrix::operator*=(const Matrix& mat)
{
    assert(cols == mat.rows);
    // After multiple we will have arr[rows][mat.cols]
    Matrix tMat{ rows,mat.cols };
    for (int iRows = 0; iRows < rows; iRows++)
    {
        for (int iCols = 0; iCols < tMat.cols; iCols++)
        {
            for (int iRun = 0; iRun < cols; iRun++)
            {
                tMat.arr[iRows][iCols] += arr[iRows][iRun] * mat.arr[iRun][iCols];
            }
        }
    }
    return *this = tMat;
}
Matrix& Matrix::operator*=(const double& num)
{
    for (int iRows = 0; iRows < rows; iRows++)
    {
        for (int iCols = 0; iCols < cols; iCols++)
        {
            
            arr[iRows][iCols] = arr[iRows][iCols] * num;
        }
    }
    return *this;
}
Matrix& Matrix::operator/=(const double& num)
{
    for (int iRows = 0; iRows < rows; iRows++)
    {
        for (int iCols = 0; iCols < cols; iCols++)
        {
            
             arr[iRows][iCols] = arr[iRows][iCols] / num;
        }
    }
    return (*this);
}
Matrix& Matrix::operator=(const Matrix& mat)
{
    if (this == &mat)
        return *this;

    if (rows != mat.rows || cols != mat.cols)
    {
        // delete old
        for (int iRows = 0; iRows < rows; iRows++)
        {
            delete[] arr[iRows];
        }
        delete[] arr;

        // initializing new space
        rows = mat.rows;
        cols = mat.cols;
        allocSpace();
    }

    for (int iRows = 0; iRows < rows; iRows++)
    {
        for (int iCols = 0; iCols < cols; iCols++)
        {
            arr[iRows][iCols] = mat.arr[iRows][iCols];
        }
    }

    return *this;
}


// *************************** Functions ********************************


/*
General Algorithm:
      0  1   2
    0 | 3  4  5 |                0| 2   3  |      |  1   3 |       | 1   2 |
A = 1 | 1  2  3 | => det(A) = 3*  |        | - 4* |        |  + 5* |       |
    2 | 1  3  2 |                1| 3   2  |      |  1   2 |       | 1   3 |

=> det(A) = 3*(2*2 - 3*3) - 4*(1*2 - 3*1) + 5*(1*3 - 1*2)
=> det(A) = -15 + 4 + 5 = 6

*/
Matrix cofactor(const Matrix& mat,const int& rowAvoid, const int& colAvoid)
{
    Matrix temp{ mat.getCols() - 1, mat.getRows() - 1 };
    double res = 0;
    double** tArr = mat.getArr();
    int iRowOut = 0;
    int iColOut = 0;
    for (int iRows = 0; iRows < mat.getRows(); iRows++)
    {
        for (int iCols = 0; iCols < mat.getCols(); iCols++)
        {
            if (iRows != rowAvoid && iCols != colAvoid)
            {
                temp.getArr()[iRowOut][iColOut++] = mat.getArr()[iRows][iCols];
            }
            if (iColOut == mat.getCols() - 1)
            {
                iColOut = 0;
                iRowOut++;
            }
        }
    }
    return temp;
}
double determinant(const Matrix& mat)
{
    if (mat.getCols() == 1)
    {
        double temp = mat.getArr()[0][0];
        return temp;
    }
    int N = mat.getCols();
    double D = 0;
    int sign = 1;
    Matrix tMat{ mat.getCols() - 1, mat.getCols() - 1};
    for (int iCols = 0; iCols < N; iCols++)
    {
        tMat = cofactor(mat, 0, iCols);
        D += sign * mat.getArr()[0][iCols] * determinant(tMat);
        sign = -sign;
    }
    return D;
}
/*
       *********************How to find Adjoint?******************
We follow definition given above.
Let A[N][N] be input matrix.
1) Create a matrix adj[N][N] store the adjoint matrix.
2) For every entry A[i][j] in input matrix where 0 <= i < N
   and 0 <= j < N.
    a) Find cofactor of A[i][j]
    b) Find sign of entry.  Sign is + if (i+j) is even else
       sign is odd.
    c) Place the cofactor at adj[j][i]
*/
Matrix adjoint(const Matrix& mat)
{
    assert(mat.getCols() == mat.getRows());
    Matrix adj{ mat.getCols(),mat.getCols() };
    if (mat.getCols() == 1)
    {
        adj.getArr()[0][0] = mat.getArr()[0][0];
        return adj;
    }
    
    int sign = 1;
    Matrix tMat{ mat.getRows() - 1,mat.getCols() - 1};

    for (int iRows = 0; iRows < mat.getRows(); iRows++)
    {
        for (int iCols = 0; iCols < mat.getCols(); iCols++)
        {
            // get cofactor:
            tMat = cofactor(mat, iRows, iCols);

            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((iRows + iCols) % 2 == 0) ? 1 : -1;

            adj.getArr()[iCols][iRows] = (sign) * (determinant(tMat));
        }
    }
    return adj;
}
/*
To find A-1 of A:
-> make sure that det(A) != 0;
-> If so, we can find A-1 = adj(A)/det(A);

Summary:
  If det(A) != 0
    A-1 = adj(A)/det(A)
  Else
    "Inverse doesn't exist"

   */
Matrix inverse(const Matrix& mat)
{
    assert(mat.getCols() == mat.getRows());
   
    int N = mat.getCols();
    Matrix inver{ N,N };
    Matrix _adj = adjoint(mat);
    double _det = determinant(mat);

    if (_det == 0)
    {
        cout << "Inverse doesn't exist!";
        return inver;
    }
    else
    {
        inver = _adj / _det;
        return inver;
    }
}


Matrix Sum(const Matrix& mat1, const Matrix& mat2)
{   
    Matrix Temp { mat1 };
    return (Temp + mat2);
}
Matrix Sub(const Matrix& mat1, const Matrix& mat2)
{
    return mat1 - mat2;
}
Matrix MulMatrix(const Matrix& mat1, const Matrix& mat2)
{
    return mat1 * mat2;
}
Matrix MulNumber(const Matrix& mat, const double& number)
{
    return mat * number;
}
Matrix DivMatrix(const Matrix& mat1, const Matrix& mat2)
{
    return mat1 / mat2;
}
Matrix DivNumber(const Matrix& mat, const double& num)
{
    return mat / num;
}
void printConsole()
{
    cout << setw(50) << "************************ CONSOLE MATRIX ***********************" << endl;
    cout << setw(50) << "*                1.Inputing for two Matrices                  *" << endl;
    cout << setw(50) << "*                2.Calculating Sum of two Matrices            *" << endl;
    cout << setw(50) << "*                3.Subtraction of two Matrices                *" << endl;
    cout << setw(50) << "*                4.Multiplication of Matrices                 *" << endl;
    cout << setw(50) << "*                5.Division of Matrices                       *" << endl;
    cout << setw(50) << "*                0.Exit                                       *" << endl;
    cout << setw(50) << "***************************************************************" << endl;
}
void MultiConsole()
{
    cout << endl;
    cout << setw(50) << "*************** CONSOLE: Multiplication MATRIX ****************" << endl;
    cout << setw(50) << "*               0.Multiplication for two Matrices             *" << endl;
    cout << setw(50) << "*               1.Multiplication for a double number          *" << endl;
    cout << setw(50) << "***************************************************************" << endl;
}
void DivConsole()
{
    cout << endl;
    cout << setw(50) << "****************** CONSOLE: Division MATRIX *******************" << endl;
    cout << setw(50) << "*               0.Division for two Matrices                   *" << endl;
    cout << setw(50) << "*               1.Division for a double number                *" << endl;
    cout << setw(50) << "***************************************************************" << endl;
    cout << endl;
}
