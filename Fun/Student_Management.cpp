#pragma warning(disable : 4996)

#include <iostream>
#include <cstring>

using namespace std;

#define MAXSIZE 100

struct SinhVien
{
	char Account[10]{};
	char first_name[10]{};
	char last_name[10]{};
	int MSSV{};
};

void printLine(int a)
{
	for (int i = 0; i < a; i++)
		cout << "*";
	cout << "\n";
}

bool checkAccount(char account_1[10], char account_2[10])
{
	for (int i = 0; i < 10; i++)
	{
		std::cout << "checking " << i << " times!\n";
		if (account_1[i] != account_2[i])
			return false;
	}
	return true;
}
void Menu()
{
	std::cout << "***************** OPTIONS LIST **********************" << std::endl;
	std::cout << "1. Enter information of student" << std::endl;
	std::cout << "2. Print information of all student" << std::endl;
	std::cout << "3. Searching by account!" << std::endl;
}

void enterInfo(SinhVien sv[], int& sv_numbers)
{
	printLine(30);
	
	cout << "Enter infor of sinh vien: " << sv_numbers + 1 << "\n";

	cout << "Enter Account: ";
	char* account = new char;
	cin.getline(account, 10);
	strcpy_s(sv[sv_numbers].Account, account);
	cin.sync(); // clear buffer

	cout << "Enter first name: ";
	char* firs_name = new char;
	cin.getline(firs_name, 10);
	strcpy_s(sv[sv_numbers].first_name, firs_name);
	cin.sync();

	cout << "Enter last name: ";
	char* last_name = new char;
	cin.getline(last_name, 10);
	strcpy_s(sv[sv_numbers].last_name, last_name);
	cin.sync();

	cout << "Enter MSSV: ";
	cin >> sv[sv_numbers].MSSV;
	cin.sync();


	sv_numbers++;
}

void printInfo(SinhVien sv[], int sv_numbers)
{

	cout << "STT " << "\t" << "Account: " << " \t" << "Fir_name " << " \t" << "Las_Name " << " \t" << " MSSV \n";
	for (int i = 0; i < sv_numbers; i++)
	{
		cout << " " << i + 1 << "\t";
		cout << sv[i].Account << "\t";
		cout << sv[i].first_name << "\t \t";
		cout << sv[i].last_name << "\t \t";
		cout << sv[i].MSSV << " \n";

		cout << "\n";
	}
}


void Searching(SinhVien sv[], int sv_numbers)
{
	cout << "\nEnter account - searching: ";
	char account_find[10]{};
	cin.getline(account_find, 10);
	cin.sync();

	for (int index = 0; index < sv_numbers; index++)
	{
		if (checkAccount(sv[index].Account,account_find))
		{
			cout << "Found!\n";
			cout << "Account: " << sv[index].Account << "\n";
			cout << "First_name: " << sv[index].first_name << "\n";
			cout << "Last_name: " << sv[index].last_name << "\n";
			cout << "MSSV: " << sv[index].MSSV << "\n";
		}
	}
	std::cout << "Not Found! " << std::endl;
}
	


int main()
{
	SinhVien sv[MAXSIZE];
	int sv_numbers = 0;
	std::cout << "WELCOME TO MY STUDENT MANAGEMENT PROJECT!\n";
	while (true)
	{

		Menu();
		printLine(40);
		int choice{};
		std::cout << "Enter your choice!\n";
		std::cin >> choice;
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice)
		{
		case 1:
			enterInfo(sv, sv_numbers);
			break;
		case 2:
			printInfo(sv, sv_numbers);
			break;
		case 3:
			Searching(sv, sv_numbers);
			break;
		default:
			break;
		}
	}
	
	return 0;
}
