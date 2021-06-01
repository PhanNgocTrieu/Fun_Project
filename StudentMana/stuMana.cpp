#pragma warning(disable : 4996)
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <iomanip>
#include <cassert>

using namespace std;
class svVector;
struct Student
{
    string Account;
    string firName;
    string lasName;
    int svID;
    Student() { Account = ""; firName = ""; lasName = ""; svID = 0; }
    Student(const string& ipAcc, const string& ipFiName, const string& ipLaName, const int& ipID)
        : Account{ ipAcc }, firName{ ipFiName }, lasName{ ipLaName }, svID{ ipID } {}
    void print()
    {
        cout << "MSSV: " << svID << endl;
        cout << "Account: " << Account << endl;
        cout << "First Name: " << firName << endl;
        cout << "Last Name: " << lasName << endl;
        cout << endl;
    };
};

bool checkMSSV(const int& MSSV_check)
{
    if (MSSV_check < 100000)
        return false;
    return true;
}

class svVector
{
private:
    Student* svList;
    int __size {0};
public:
    svVector() { };
    svVector(Student* ipSV, const int& ipSize) : svList{ ipSV }, __size{ ipSize }
    {}
    void setAcc(const string&);
    void setFirName(const string&);
    void setLasName(const string&);
    void setID(const int&);
    string getAcc();
    string getFirName();
    string getLasName();
    int getID();
    int getSize();
    void remove(const int&);
    void removeMSSV(const int&);
    void insert(const Student&, const int&);
    void push_back(const Student&);
    void pop_back();
    void findACC(const string&);
    void findMSSV(const int&);
    void print();
};

void svVector::setAcc(const string& acc)
{
    svList->Account = acc;
}
void svVector::setFirName(const string& FirName)
{
    svList->firName = FirName;
}
void svVector::setLasName(const string& LasName)
{
    svList->lasName = LasName;
}
void svVector::setID(const int& ipID)
{
    svList->svID = ipID;
}
string svVector::getAcc()
{
    return svList->Account;
}
string svVector::getFirName()
{
    return svList->firName;
}
string svVector::getLasName()
{
    return svList->lasName;
}
int svVector::getID()
{
    return svList->svID;
}
int svVector::getSize() {
    return __size;
}

void svVector::insert(const Student& sv, const int& __pos)
{
    if (__pos < 0 && __pos > __size)
    {
        cout << "Invalid position!" << endl;
        return; 
    }
    Student *svList_temp = svList;
    Student *tSvList = new Student[__size + 1];
    int cur_index = 0;
    int run_index = 0;
    while(run_index < __size)
    {
        if (run_index == __pos)
        {
            tSvList[cur_index] = sv;
            cur_index++;
        }
        {
            tSvList[cur_index] = svList[run_index];
            run_index++;
            cur_index++;
        }
    }
    __size++;
    svList = tSvList;
    delete[] svList_temp;
    svList_temp = NULL;
}
void svVector::remove(const int& __pos)
{
    // make sure that __pos of elem of vector you want to find in range
    assert(__pos > 0 && __pos < __size);
    //cout << "size:  " << __size << endl;
    Student* temp_svList = new Student[__size - 1];
    int cur_index = 0;
    int run_index = 0;
    while (cur_index < __size)
    {
        if (cur_index == __pos)
        {
            cur_index++;
        }
        else
        {
            temp_svList[run_index] = svList[cur_index];
            run_index++;
            cur_index++;
        }
    }
    svList = temp_svList;
    __size--; // luu index cua sv
}
void svVector::push_back(const Student& sv)
{
    if (getSize() == 0)
    {
        Student *tList = new Student[__size + 1];
        tList[0] = sv;
        svList = tList;
        __size++;
    }
    else
    {
        Student *tList = new Student[__size + 1];
        int i = 0;
        while(i < __size)
        {
            tList[i] = svList[i];
            i++;
        }
        tList[i] = sv;
        svList = tList;
        __size++;
    }
}
void svVector::print()
{
    cout << "Tale of SinhVien:\n";
	cout << "STT " << setw(20) << "MSSV" << setw(20) << "Account " << setw(20) << "Fir_name " << setw(20) << "Las_Name " << endl;
    for (int index = 0; index < __size; index++)
    {
        cout << index << setw(20) << svList[index].svID << setw(20) << svList[index].Account 
        << setw(20) << svList[index].firName << setw(20) << svList[index].lasName << endl;
    }
}
void svVector::pop_back()
{
    Student * tList = new Student[__size - 1];
    int index = 0;
    while (index < __size - 1)
    {
        tList[index] = svList[index];
        index++;
    }
    Student *p = svList;  
    delete[] p;
    p = NULL; // k cho tro toi thang nao ca
    cout << p << endl;
    svList = tList;
    __size--; 
}
void svVector::findACC(const string& acc)
{
    for (int index = 0; index < __size; index++)
    {
        if (svList[index].Account == acc)
        {
            svList[index].print();
            return;
        }
    }
}
void svVector::findMSSV(const int& MSSV_find)
{
    for (int index = 0; index < __size; index++)
    {
        if (svList[index].svID == MSSV_find)
        {
            svList[index].print();
            return;
        }
    }
}
void svVector::removeMSSV(const int& MSSV)
{
    assert(__pos > 0 && __pos < __size);
    //cout << "size:  " << __size << endl;
    Student* temp_svList = new Student[__size - 1];
    int cur_index = 0;
    int run_index = 0;
    while (cur_index < __size)
    {
        if (svList[cur_index].svID == MSSV)
        {
            cur_index++;
        }
        else
        {
            temp_svList[run_index] = svList[cur_index];
            run_index++;
            cur_index++;
        }
    }
    svList = temp_svList;
    __size--; // luu index cua sv
}
Student iniSV()
{
    Student svEnter;
    cout << "Enter information of Sinh Vien you wan to insert!" << endl;
    cout << "Enter Account: ", cin >> svEnter.Account;
    cout << "Enter First Name: ", cin >> svEnter.firName;
    cout << "Enter Last Name: ", cin >> svEnter.lasName;
    cout << "Enter MSSV: ", cin >> svEnter.svID;
    return svEnter;
}
void Menu()
{
	std::cout << setw(69) << "***************** OPTIONS LIST **********************" << std::endl;
	std::cout << setw(59) << "1. Insert information of student at back" << std::endl;
    std::cout << setw(67) << "2. Insert information of student at any position" << std::endl;
	std::cout << setw(54) << "3. Print information of all student" << std::endl;
	std::cout << setw(43) << "4. Searching by account!" << std::endl;
	std::cout << setw(39) << "5. Deleting by MSSV!" << std::endl;
	std::cout << setw(27) << "0. Exit!" << std::endl;
}

int main()
{
    svVector svList;
    Student s;
    int pos;
    string account_find;
    int MSSV_find;
    while(true)
    {
        Menu();
        char choice;
        cout << "Enter your choice: ", cin >> choice;
        switch (choice)
        {
        case '1':
            cout << "You did choose 1" << endl;
            s = iniSV();
            svList.push_back(s);
            cout << "You already push new student into data base!" << endl;
            system("cls");
            break;
        case '2':
            cout << "You did choose 2" << endl;
            cout << "Enter pos you wanna insert: ", cin >> pos;
            s = iniSV();
            svList.insert(s,pos);
            cout << "You already push new student into data base!" << endl;
            system("cls");
            break;
        case '3':
            cout << "You did choose 3" << endl;
            cout << endl << "Output:" << endl;
            svList.print();
            cout << endl;
            system("cls");
            break;
        case '4':
            cout << "You did choose 4" << endl;
            cout << "Enter Account to find: ", cin >> account_find;
            cout << endl << "Output:" << endl;
            svList.findACC(account_find);
            system("cls");
            break;
        case '5':
            cout << "You did choose 5" << endl;
            cout << "Enter MSSV to find: ", cin >> MSSV_find;
            svList.removeMSSV(MSSV_find);
            system("cls");
        case '0':
            cout << "You did choose 0" << endl;
            cout << "goodbye!" << endl;
            return 0;
        default:
            cout << "What do you mean? Choose Again!" << endl;
            break;
        }
    }

    return 0;
}
