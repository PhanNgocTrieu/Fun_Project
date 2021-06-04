#pragma warning(disable: 6289)
#include "Matrix.h"
#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	Matrix mat1;
	Matrix mat2;
	Matrix matSum;
	Matrix matSub;
	Matrix matMul;
	Matrix matDiv;
	char anotherChoice{};
	//char mulChoice{};
	//char divChoice{};
	int choice{};
	while (true)
	{
		printConsole();
		cout << "Enter your choice: ", cin >> choice;
		while(cin.fail())
		{
			cerr << "Error!" << endl;
			cout << "Enter again (just number!): ", cin >> choice;
		}
		switch (choice)
		{
		case 1:
Enter:
			system("cls");
			cout << "Now you Enter your first matrix: " << endl;
			mat1.ipMatrix();
			cout << endl;
			cout << "Now your enter your second matrix: " << endl;
			mat2.ipMatrix();
			cout << endl;
			cout << "This is matrix 1: " << mat1;
			cout << "This is matrix 2: " << mat2;
			cout << endl << endl << "Take a look!" << endl;
			Sleep(3000);		
			cout << "Do you want to enter matrix again? [Y/N] ";
			cin >> anotherChoice;
			while ( anotherChoice != 'Y' && anotherChoice != 'y' && anotherChoice != 'N' && anotherChoice != 'n')
			{
				cout << "Error!" << endl;
				cout << "Enter again, please! (just Y or N)!", cin >> anotherChoice;
			}
			if (anotherChoice == 'Y')
			{
				system("cls");
				goto Enter;
			}
			else
			{
				system("cls");
				break;
			}	

		case 2:
			matSum = Sum(mat1, mat2);
			cout << "Your're in calculating Sum of 2 matrices option!" << endl;
			cout << "Here is your result! " << endl;
			matSum.printMatrix();
			cout << endl;
			cout << endl << "Take a look!" << endl;
			Sleep(5000);
			system("cls");
			break;

		case 3:
			matSub = Sub(mat1, mat2);
			cout << "Your're in calculate Sub of 2 matrices option!" << endl;
			cout << "Here is your result! " << endl;
			matSub.printMatrix();
			cout << endl;
			cout << endl << "Take a look!" << endl;
			Sleep(5000);
			system("cls");
			break;

		case 4:
Mul:			
			system("cls");
			cout << "Your're in Multiplicating matrices option!" << endl;
			MultiConsole();
			cout << "Enter your choice for multiplicating!" << endl;
			cin >> anotherChoice;
			while (anotherChoice != '0' && anotherChoice != '1')
			{
				cerr << "Error!" << endl;
				cout << "Please enter again (just 0 - 1): ", cin >> anotherChoice;
			}
			if (anotherChoice == '0')
			{
				system("cls");
				cout << "Mat 1: ";
				mat1.printMatrix();
				cout << endl;
				cout << "Mat 2: ";
				mat2.printMatrix();
				cout << endl;
				cout << "Result!";
				matMul = MulMatrix(mat1, mat2);
				matMul.printMatrix();
				cout << endl << "Take look your resulting! " << endl;
				Sleep(2000);
				cout << "Do you want continuing to take Mul option? [Y/N]" << endl;
				cin >> anotherChoice;
				while (anotherChoice != 'Y' && anotherChoice != 'N')
				{
					cerr << "Error!" << endl;
					cout << "Please enter again (just Y - N): ", cin >> anotherChoice;
				}
				if (anotherChoice == 'Y')
				{
					goto Mul;
				}
				else
				{
					system("cls");
					break;
				}
			}
			else
			{
				double number;
				cout << "Enter number: ", cin >> number;
				cout << "Below is resul: " << endl;
				matMul = MulNumber(mat1, number);
				matMul.printMatrix();
				matMul = MulNumber(mat2, number);
				matMul.printMatrix();
				cout << endl << "Take look your resulting! " << endl;
				Sleep(2000);
				cout << "Do you want continuing to take Mul option? [Y/N]" << endl;
				cin >> anotherChoice;
				while (anotherChoice != 'Y' && anotherChoice != 'N')
				{
					cerr << "Error!" << endl;
					cout << "Please enter again (just Y - N): ", cin >> anotherChoice;
				}
				if (anotherChoice == 'Y')
				{
					goto Mul;
				}
				else
				{
					system("cls");
					break;
				}
			}
		case 5:
Div:
			system("cls");
			cout << "Your're in Dividing matrices option!" << endl;
			DivConsole();
			cout << "Enter your choice for multiplicating!" << endl;
			cin >> anotherChoice;
			while (anotherChoice != '0' && anotherChoice != '1')
			{
				cerr << "Error!" << endl;
				cout << "Please enter again (just 0 - 1): ", cin >> anotherChoice;
			}
			if (anotherChoice == '0')
			{
				system("cls");
				cout << "Mat 1: ";
				mat1.printMatrix();
				cout << endl;
				cout << "Mat 2: ";
				mat2.printMatrix();
				cout << endl;
				cout << "Result!";
				matDiv = MulMatrix(mat1, mat2);
				matDiv.printMatrix();
				cout << endl << "Take look your resulting! " << endl;
				Sleep(2000);
				cout << "Do you want continuing to take Div option? [Y/N]" << endl;
				cin >> anotherChoice;
				while (anotherChoice != 'Y' && anotherChoice != 'N')
				{
					cerr << "Error!" << endl;
					cout << "Please enter again (just Y - N): ", cin >> anotherChoice;
				}
				if (anotherChoice == 'Y')
				{
					goto Div;
				}
				else
				{
					system("cls");
					break;
				}
			}
			else
			{
				double number;
				cout << "Enter number: ", cin >> number;
				cout << "Below is resul: " << endl;
				matDiv = DivNumber(mat1, number);
				matDiv.printMatrix();
				matDiv = DivNumber(mat2, number);
				matDiv.printMatrix();
				cout << endl << "Take look your resulting! " << endl;
				Sleep(3000);
				cout << "Do you want continuing to take Div option? [Y/N]" << endl;
				cin >> anotherChoice;
				while (anotherChoice != 'Y' && anotherChoice != 'N')
				{
					cerr << "Error!" << endl;
					cout << "Please enter again (just Y - N): ", cin >> anotherChoice;
				}
				if (anotherChoice == 'Y')
				{
					goto Div;
				}
				else
				{
					system("cls");
					break;
				}
			}
		case 0:
			cout << "Thanks for visiting us!" << endl;
			Sleep(2000);
			cout << "See ya!!!!" << endl;
			Sleep(2000);
			system("cls");
			return 0;
		default:
			cout << "What do you mean? Enter again!" << endl;
			cout << "Enter your choice gain, Please!" << endl;
			Sleep(3000);
			system("cls");
			break;
		}

	}

	return 0;
}
