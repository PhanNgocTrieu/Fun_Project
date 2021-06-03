#include "Matrix.h"
#include <iostream>
using namespace std;
int main()
{
	Matrix mat;
	//cout << "go here!" << endl;
	/*cout << "\n********************* Enter **********************" << endl;
	mat.ipMatrix();
	cout << "\n********************* Print **********************" << endl;
	mat.printMatrix();

	cout << "\n********************* Multiple **********************" << endl;
	Matrix mat2 = mat * 10;
	mat2.printMatrix();

	cout << "\n********************* Subtraction **********************" << endl;
	Matrix mat3 = mat2 - mat;
	mat3.printMatrix();

	cout << "\n********************* Addition **********************" << endl;
	Matrix mat4 = mat3 - mat;
	mat4.printMatrix();


	cout << "\n********************* Division **********************" << endl;
	Matrix mat5 = mat2 / 3;
	mat5.printMatrix();*/


	cout << "\n********************* Division 2 Matrix **********************" << endl;
	Matrix mat6;
	mat6.ipMatrix();
	mat6.printMatrix();
	Matrix mat7;
	mat7.ipMatrix();
	mat7.printMatrix();


	/*double temp = mat7.getArr()[0][0];
	cout << temp << endl;*/

	Matrix mat8 = mat6 / mat7;
	//mat8.printMatrix();


	return 0;
}
