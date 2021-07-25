// SqliteDoing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sqlite3.h>
#include <string>
#include "SQLITE.h"
#include "Menu.h"

using namespace std;


int main()
{
	SQLITE mySQL;
	MENU myMenu;
	bool fail;
	int choice_outside;
	int choice_inside;
	bool out;
	bool isPut = false;
	// INITIALIZING DB
	mySQL.createDatabase("myDB.db");
	mySQL.dropTables();
	mySQL.createThreeTables();

	do
	{
		system("cls");
		myMenu.mainMenu();
		
		do
		{
			fail = false;
			cout << "\nEnter your choice: ", cin >> choice_outside;
			if (cin.fail())
			{
				cout << "Wrong choice!" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				fail = true;
			}
			
		} while (fail);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice_outside)
		{
		case 1: /* ******** ADDING CASE ****** */

			/* ********************* HANDLING ADDING OPTION ***************** */

			do
			{
				system("cls");
				myMenu.addingMenu();

				out = true;
				do
				{
					fail = false;
					cout << "Enter your choice: ", cin >> choice_inside;
					if (cin.fail())
					{
						cout << "Wrong choice!" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						fail = true;
					}
				} while (fail);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				switch (choice_inside)
				{
				case 1: // salesMan Table
					mySQL.insertSalesManTable();
					if (!isPut)
						isPut = true;

					break;

				case 2: // Order Table
					mySQL.insertOrderTable();
					if (!isPut)
						isPut = true;
					break;

				case 3: // Customer Table
					mySQL.insertCustomerTable();
					if (!isPut)
						isPut = true;
					break;

				case 4: // Automatically

					if (!isPut)
					{
						mySQL.insertAutomatically();
						isPut = true;
					}
					else
						cout << "Just using this when empty!" << endl;

					break;

				case 0:
					break;
				default:
					out = false;
					cout << "No Option!" << endl << endl;
					system("pause");
					break;
				}
			} while (!out);
			
			system("pause");
			break;

		case 2: /* ******** UPDATING CASE ****** */

			/* ********************* HANDLING ADDING OPTION ***************** */
			do
			{
				system("cls");
				myMenu.updatingMenu();
				out = true;
				// Handling Enter choices
				do
				{
					fail = false;
					cout << "Enter your choice: ", cin >> choice_inside;
					if (cin.fail())
					{
						cout << "Wrong choice!" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						fail = true;
					}
				} while (fail);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// Handling switch cases
				switch (choice_inside)
				{
				case 1: // Update for sales Man
					mySQL.listSalesManTable();
					mySQL.updateSalesManTable();
					break;

				case 2: // Update for Order
					mySQL.listOrdersTable();
					mySQL.updateOrdersTable();
					break;

				case 3: // Update for Customer
					mySQL.listCustomerTable();
					mySQL.updateCustomerTable();
					break;
				case 0: // Out of this scope
					break;

				default: // Exception
					out = false;
					cout << "No Option!" << endl << endl;
					system("pause");
					break;
				}
			} while (!out);

			system("pause");
			break;
			
		case 3: /* ******** DELETING CASE ****** */
			
			/* ********************* HANDLING ADDING OPTION ***************** */
			do
			{
				system("cls");
				myMenu.deletingMenu();
				out = true;
				// Handling Enter choices
				do
				{
					fail = false;
					cout << "Which table for handling? Enter your choice: ", cin >> choice_inside;
					if (cin.fail())
					{
						cout << "Wrong choice!" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						fail = true;
					}
				} while (fail);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// Handling switch cases
				switch (choice_inside)
				{
				case 1: /* *** SALESMAN CASE *** */
					mySQL.deleteAllSalesManTable();
					break;

				case 2: /* *** SALESMAN CASE *** */
					mySQL.deleteAllOrdersTable();
					break;

				case 3: /* *** SALESMAN CASE *** */
					mySQL.deleteAllCustomerTable();
					break;

				case 0: /* *** Out Of Scope *** */
					break;

				default: /* *** Exception CASE *** */
					out = false;
					cout << "No Option!" << endl << endl;
					system("pause");
					break;
				}
			} while (!out);

			system("pause");
			break;


		case 4: /* ******** DISPLAYING CASE ****** */
			do
			{
				system("cls");
				myMenu.displayMenu();
				out = true;
				// Handling Enter choices
				do
				{
					fail = false;
					cout << "Which table for handling? Enter your choice: ", cin >> choice_inside;
					if (cin.fail())
					{
						cout << "Wrong choice!" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						fail = true;
					}
				} while (fail);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');


				// Handling switch cases
				switch (choice_inside)
				{
				case 1:
					system("cls");
					mySQL.listSalesManTable();
					break;

				case 2:
					system("cls");
					mySQL.listOrdersTable();
					break;
				case 3:
					system("cls");
					mySQL.listCustomerTable();
					break;
				case 4:
					system("cls");
					mySQL.listAllOfTable();
					break;
				case 0: // Out of this scope
					break;
				default:
					out = false;
					cout << "No Option!" << endl << endl;
					break;
				}
			} while (!out);

			system("pause");
			
			break;

		case 5: /* ******** unOrderName CASE ****** */
			system("cls");
			mySQL.unOrderName();
			cout << endl;
			system("pause");
			break;

		case 6: /* ******** Creating Tables CASE ****** */
			do
			{
				system("cls");
				myMenu.creatingMenu();
				out = true;
				// Handling Enter choices
				do
				{
					fail = false;
					cout << "Which table for handling? Enter your choice: ", cin >> choice_inside;
					if (cin.fail())
					{
						cout << "Wrong choice!" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						fail = true;
					}
				} while (fail);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// Handling switch cases
				switch (choice_inside)
				{
				case 1: // Creating salesMan Table
					system("cls");
					mySQL.createSalesManTable();
					break;

				case 2: // Creating Orders Table
					system("cls");
					mySQL.createOrderTable();
					break;

				case 3: // Creating Customer Table
					system("cls");
					mySQL.createCustomerTable();
					break;

				case 4: // Creating 3 Tables
					system("cls");
					mySQL.createThreeTables();
					break;

				case 0: // Out of this scope
					break;

				default:
					out = false;
					cout << "No Option!" << endl << endl;
					break;
				}
			} while (!out);

			system("pause");
			break;

		case 7: /* ******** Reset Tables CASE ****** */
			system("cls");
			mySQL.resetTables();
			isPut = false;
			system("pause");
			break;
		case 0:
			return 0;
		default:
			break;
		}

	} while (true);


	
	//mySQL.insertAutomatically();
	
	// ** INSERT VALUES BY HAND
	//mySQL.insertSalesManTable();
	//mySQL.insertOrderTable();
	//mySQL.insertCustomerTable();


	// ** UPDATE VALUES
	//mySQL.listSalesManTable(); // seeing for data to change
	//mySQL.updateSalesManTable();
	
	//mySQL.listOrdersTable();
	//mySQL.updateOrdersTable();

	//mySQL.listCustomerTable();
	//mySQL.updateCustomerTable();


	// ** SELECT QUERY FROM 3 TABLES:
	//mySQL.listSalesManTable();
	//mySQL.listOrdersTable();
	//mySQL.listCustomerTable();
}


