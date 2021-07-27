#include "Menu.h"
#include <iostream>
#include <iomanip>

using namespace std;

void MENU::mainMenu()
{
	cout << "\t\t" << " *************** SALE MANAGEMENT **************** " << endl;
	cout << "\t\t" << "*              1. Adding                         *" << endl;
	cout << "\t\t" << "*              2. Update                         *" << endl;
	cout << "\t\t" << "*              3. Delete                         *" << endl;
	cout << "\t\t" << "*              4. Display                        *" << endl;
	cout << "\t\t" << "*              5. unOrder Name                   *" << endl;
	cout << "\t\t" << "*              6. Create Table                   *" << endl;
	cout << "\t\t" << "*              7. Reset Tables                   *" << endl;
	cout << "\t\t" << "*              0. Exit                           *" << endl;
	cout << "\t\t" << " ************************************************ " << endl;
}

void MENU::creatingMenu()
{
	cout << "\t\t" << " *************** Creating OPTIONS *************** " << endl;
	cout << "\t\t" << "*                1. new salesMan                 *" << endl;
	cout << "\t\t" << "*                2. new Order                    *" << endl;
	cout << "\t\t" << "*                3. new Customer                 *" << endl;
	cout << "\t\t" << "*                4. Three Tables                 *" << endl;
	cout << "\t\t" << "*                0. Exit                         *" << endl;
	cout << "\t\t" << " ************************************************ " << endl;
}

void MENU::addingMenu()
{
	cout << "\t\t" << " ***************  ADDING OPTIONS  *************** " << endl;
	cout << "\t\t" << "*                1. new salesMan                 *" << endl;
	cout << "\t\t" << "*                2. new Order                    *" << endl;
	cout << "\t\t" << "*                3. new Customer                 *" << endl;
	cout << "\t\t" << "*                4. Automatically                *" << endl;
	cout << "\t\t" << "*                0. Exit                         *" << endl;
	cout << "\t\t" << " ************************************************ " << endl;
}

void MENU::deletingMenu()
{
	cout << "\t\t" << " *************  DELETING DATA OPTIONS  ********** " << endl;
	cout << "\t\t" << "*                1. delete salesMan             *" << endl;
	cout << "\t\t" << "*                2. delete Order                *" << endl;
	cout << "\t\t" << "*                3. delete Customer             *" << endl;
	cout << "\t\t" << "*                0. Exit                        *" << endl;
	cout << "\t\t" << " *********************************************** " << endl;
}

void MENU::insideDeletingMenu()
{
	cout << "\t\t" << " ********* Insided Deleting Data OPTIONS  ****** " << endl;
	cout << "\t\t" << "*                1. delete salesMan             *" << endl;
	cout << "\t\t" << "*                2. delete Order                *" << endl;
	cout << "\t\t" << "*                3. delete Customer             *" << endl;
	cout << "\t\t" << "*                4. delete data of all tables   *" << endl;
	cout << "\t\t" << "*                0. Exit                        *" << endl;
	cout << "\t\t" << "************************************************ " << endl;
}


void MENU::updatingMenu()
{
	cout << "\t\t" << " **********  UPDATING DATA OPTIONS  ********** " << endl;
	cout << "\t\t" << "*                1. update salesMan           *" << endl;
	cout << "\t\t" << "*                2. update Order              *" << endl;
	cout << "\t\t" << "*                3. update Customer           *" << endl;
	cout << "\t\t" << "*                0. Exit                      *" << endl;
	cout << "\t\t" << " ********************************************* " << endl;
}

void MENU::displayMenu()
{
	cout << "\t\t" << " **********  DIPLAYING DATA OPTIONS  ********* " << endl;
	cout << "\t\t" << "*                1. display salesMan          *" << endl;
	cout << "\t\t" << "*                2. display Order             *" << endl;
	cout << "\t\t" << "*                3. display Customer          *" << endl;
	cout << "\t\t" << "*                4. display Whole Tables      *" << endl;
	cout << "\t\t" << "*                0. Exit                      *" << endl;
	cout << "\t\t" << " ********************************************* " << endl;
}
