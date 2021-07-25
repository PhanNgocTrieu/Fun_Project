#include "SQLITE.h"
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


int SQLITE::Customer_ID = 0;
int SQLITE::salesMan_ID = 0;
int SQLITE::Order_No = 0;


SQLITE::SQLITE()
{
}

// parameter is path to DB -> create DB
SQLITE::SQLITE(const string& pathToDB)
{
	this->pathToDB = pathToDB;
	this->createDatabase(pathToDB);
}

SQLITE::SQLITE(const SQLITE& _other)
{
	this->pathToDB = _other.pathToDB;
}

SQLITE::~SQLITE()
{
}

void SQLITE::createDatabase(const string& pathToDB)
{
	int rc;
	sqlite3* DB;
	this->pathToDB = pathToDB;
	rc = sqlite3_open(pathToDB.c_str(), &DB);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error Open Database!\n");
	}
	else
	{
		fprintf(stdout, "Open Database successfully!\n");
	}
	sqlite3_close(DB);
}

void SQLITE::createSalesManTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("CREATE TABLE IF NOT EXISTS salesMan("
		"salesMan_ID INTEGER  NOT NULL PRIMARY KEY,"
		"Name VARCHAR(30)        NOT NULL,"
		"City VARCHAR(15)        NOT NULL,"
		"Commission DECIMAL(5, 2) NOT NULL"
		");"
	);

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nCreated Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::createOrderTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("CREATE TABLE IF NOT EXISTS Orders("
		"Order_No INTEGER	NOT NULL PRIMARY KEY,"
		"Purch_AMT DECIMAL(8, 2)  NOT NULL,"
		"Order_Date DATE         NOT NULL,"
		"Customer_ID NUMERIC(5)  NOT NULL,"
		"salesMan_ID NUMERIC(5) 	NOT NULL,"
		"FOREIGN KEY(salesMan_ID) REFERENCES salesMan(salesMan_ID),"
		"FOREIGN KEY(Customer_ID) REFERENCES Customer(Customer_ID)"
		");"
	);

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nCreated Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::createCustomerTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("CREATE TABLE IF NOT EXISTS Orders("
		"Customer_ID INTEGER	NOT NULL PRIMARY KEY,"
		"Cust_Name   VARCHAR(30) NOT NULL,"
		"City        VARCHAR(15) NOT NULL,"
		"Grade       NUMERIC(3)  NOT NULL,"
		"salesMan_ID NUMERIC(5)  NOT NULL,"
		"FOREIGN KEY(salesMan_ID) REFERENCES salesMan(salesMan_ID)"
		");"
	);

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nCreated Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::createThreeTables()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("CREATE TABLE IF NOT EXISTS salesMan("
		"salesMan_ID INTEGER  NOT NULL PRIMARY KEY,"
		"Name VARCHAR(30)        NOT NULL,"
		"City VARCHAR(15)        NOT NULL,"
		"Commission DECIMAL(5, 2) NOT NULL"
		");"

		"CREATE TABLE IF NOT EXISTS Orders("
		"Order_No INTEGER	NOT NULL PRIMARY KEY,"
		"Purch_AMT DECIMAL(8, 2)  NOT NULL,"
		"Order_Date DATE         NOT NULL,"
		"Customer_ID NUMERIC(5)  NOT NULL,"
		"salesMan_ID NUMERIC(5) 	NOT NULL,"
		"FOREIGN KEY(salesMan_ID) REFERENCES salesMan(salesMan_ID),"
		"FOREIGN KEY(Customer_ID) REFERENCES Customer(Customer_ID)"
		");"

		"CREATE TABLE IF NOT EXISTS Customer("
		"Customer_ID INTEGER	NOT NULL PRIMARY KEY,"
		"Cust_Name   VARCHAR(30) NOT NULL,"
		"City        VARCHAR(15) NOT NULL,"
		"Grade       NUMERIC(3)  NOT NULL,"
		"salesMan_ID NUMERIC(5)  NOT NULL,"
		"FOREIGN KEY(salesMan_ID) REFERENCES salesMan(salesMan_ID)"
		");"
	);

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nCreated Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}





// ********************************* INSERTING ************************************************
void SQLITE::insertSalesManTable()
{
	int rc;
	sqlite3* DB;
	string sqlCommand;
	char* errMsg;

	// Increasing ID;
	SQLITE::salesManIdIncreasing();

	// Enter Value for insert!
	cout << "Enter name: ", getline(cin, this->saleManTab.Name);
	cout << "City: ", getline(cin, this->saleManTab.City);
	cout << "Commission: ", cin >> this->saleManTab.Commission;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	sqlCommand = "INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(" + to_string(SQLITE::getSalesManID()) +
		", '" + this->saleManTab.Name + "', '" + this->saleManTab.City + "', " + to_string(this->saleManTab.Commission) + ");";
	cout << "sqlCommand: " << sqlCommand << endl	;
	rc = sqlite3_open(this->pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nInsert Into Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::insertOrderTable()
{
	int rc;
	sqlite3* DB;
	string sqlCommand;
	char* errMsg;

	// Increasing ID;
	SQLITE::ordersNoIncreasing();

	// Enter Value for insert!
	cout << "Purch_AMT: ",cin >> this->orderTab.Purch_AMT;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Order_Date: ", getline(cin, this->orderTab.Order_Date);

	cout << "Customer_ID: ", cin >> this->orderTab.Customer_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "salesMan_ID: ", cin >> this->orderTab.salesMan_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	sqlCommand = "INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(" + to_string(SQLITE::getOrderNo()) + ", "
		+ to_string(this->orderTab.Purch_AMT) + ", '" + this->orderTab.Order_Date + "', " + to_string(this->orderTab.Customer_ID)
		+ ", " + to_string(this->orderTab.salesMan_ID) + ");";

	cout << "sqlCommand: " << sqlCommand << endl;


	rc = sqlite3_open(this->pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nInsert Into Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::insertCustomerTable()
{
	int rc;
	sqlite3* DB;
	string sqlCommand;
	char* errMsg;

	// Increasing ID;
	SQLITE::customerIdIncreasing();

	// Enter Value for insert!
	cout << "Customer Name: ", getline(cin, this->cusTab.Cust_Name);

	cout << "City: ", getline(cin, this->cusTab.City);

	cout << "Grade: ", cin >> this->cusTab.Grade;

	cout << "salesMan_ID: ", cin >> this->cusTab.salesMan_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	sqlCommand = "INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(" + to_string(SQLITE::getCusID()) + ", '"
		+ cusTab.Cust_Name + "', '" + cusTab.City + "', " + to_string(cusTab.Grade) + ", " + to_string(cusTab.salesMan_ID) + ");";

	cout << "sqlCommand: " << sqlCommand << endl;


	rc = sqlite3_open(this->pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nInsert Into Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}


void SQLITE::insertAutomatically()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	// Update ID :D
	
	string sqlCommand(
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(1, 'Dinh Thien Hoan', 'Quang Nam', 6);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(2, 'Cao Thi Lam', 'Quang Nam', 5);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(3, 'Sa Sa Mua Mua', 'Da Nang', 3.2);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(4, 'Sang Roi Ngu', 'Da Nang', 3.5);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(5, 'Toi Xem Phim', 'Da Nang', 2);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(6, 'Monkey D. Luffy', 'Da Nang', 5);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(7, 'Nguyen Du Lich', 'Kon Tum', 3.3);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(8, 'Cuu Hong Bao', 'Da Nang', 6.2);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(9, 'Duong Thanh Tien', 'Thanh Hoa', 5.5);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(10, 'Ngoc Lam Nguy', 'Quang Nam', 4.2);"
		"INSERT INTO salesMan(salesMan_ID, Name, City, Commission) VALUES(11, 'Vo Thi Be', 'Hue', 2.2);"

		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(1, 32000, '20-7-2021 12:00', 1, 1);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(2, 72000, '20-7-2021 12:01', 2, 2);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(3, 12000, '20-7-2021 12:02', 3, 3);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(4, 21000, '20-7-2021 12:10', 4, 4);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(5, 33000, '20-7-2021 12:20', 5, 5);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(6, 36000, '20-7-2021 12:30', 6, 6);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(7, 45000, '20-7-2021 12:40', 7, 7);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(8, 71000, '20-7-2021 12:50', 8, 8);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(9, 65000, '20-7-2021 13:00', 9, 9);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(10, 81000, '20-7-2021 13:10', 10, 10);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(11, 70000, '20-7-2021 13:20', 11, 11);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(12, 90000, '21-7-2021 13:30', 3, 2);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(13, 70000, '21-7-2021 13:50', 4, 1);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(14, 110000, '21-7-2021 14:20', 2, 2);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(15, 86000, '21-7-2021 15:20', 2, 3);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(16, 423000, '21-7-2021 16:20', 3, 2);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(17, 213000, '21-7-2021 17:20', 4, 1);"
		"INSERT INTO Orders(Order_No, Purch_AMT, Order_Date, Customer_ID, salesMan_ID) VALUES(18, 70000, '21-7-2021 18:20', 2, 2);"

		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(1, 'Phan Ngoc Trieu', 'Quang Nam', 10, 1);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(2, 'Hoang Duy Nam', 'Quang Nam', 12, 2);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(3, 'DINH PHAN THIEN', 'Da Nang', 23, 3);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(4, 'Truong Lam Le', 'Hue', 54, 4);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(5, 'Le viet Hieu', 'Da Nang', 33, 5);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(6, 'Nguyen Van Duy', 'Da Nang', 122, 6);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(7, 'Do Van Kien', 'Kon Tum', 21, 7);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(8, 'Le Xuan Dieu', 'Da Nang', 38, 8);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(9, 'Nguyen Dai Dao', 'Thanh Hoa', 81, 9);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(10, 'Tran Phuoc Dinh', 'Quang Nam', 75, 10);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(11, 'Ton That Tinh', 'Hue', 68, 11);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(12, 'Phan Trieu John', 'Quang Nam', 0, 2);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(13, 'Phan Linh An', 'Quang Nam', 0, 11);"
		"INSERT INTO Customer(Customer_ID, Cust_Name, City, Grade, salesMan_ID) VALUES(14, 'Phan Dinh Phung', 'Quang Nam', 0, 3);"
	);
	

	// Update ID 
	SQLITE::salesMan_ID = 11;
	SQLITE::Order_No = 18;
	SQLITE::Customer_ID = 14;


	rc = sqlite3_open(pathToDB.c_str(), &DB);
	
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "Insert Values Automatically - successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}



// ************************************************ SELECT QUERY *******************************************
void SQLITE::listSalesManTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	cout << "\nDISPLAY SALES_MAN TABLE!\n";
	string sqlCommand("SELECT * FROM salesMan LIMIT 1;");

	rc = sqlite3_open(pathToDB.c_str(), &DB);

	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);

	sqlCommand = "SELECT * FROM salesMan ORDER BY salesMan_ID";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	
	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::listOrdersTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand( "SELECT * FROM Orders LIMIT 1;");
	cout << "\nDISPLAY Order TABLE!\n";
	rc = sqlite3_open(pathToDB.c_str(), &DB);

	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);

	sqlCommand = "SELECT * FROM Orders";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::listCustomerTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("SELECT * FROM Customer LIMIT 1;");
	cout << "\nDISPLAY Customer TABLE!\n";
	rc = sqlite3_open(pathToDB.c_str(), &DB);

	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);

	sqlCommand = "SELECT * FROM Customer";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::listAllOfTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("SELECT * FROM salesMan LIMIT 1;");

	rc = sqlite3_open(pathToDB.c_str(), &DB);

	cout << "\t\t****************************SalesMan Table****************************" << endl;
	// ColName
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);
	// Get Value
	sqlCommand = "SELECT * FROM salesMan";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	cout << endl;



	cout << "\t\t****************************Order Table****************************" << endl;
	sqlCommand = "SELECT * FROM Orders LIMIT 1";
	// ColName
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);
	// Get Value
	sqlCommand = "SELECT * FROM Orders";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	cout << endl;



	cout << "\t\t****************************Customer Table****************************" << endl;
	sqlCommand = "SELECT * FROM Customer LIMIT 1";
	// ColName
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);
	// Get Value
	sqlCommand = "SELECT * FROM Customer";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::dropTables()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand( "DROP TABLE IF EXISTS salesMan;"
		"DROP TABLE IF EXISTS Orders;"
		"DROP TABLE IF EXISTS Customer;"
	);

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nDrop Tables successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::resetTables()
{
	deleteAllCustomerTable();
	deleteAllOrdersTable();
	deleteAllSalesManTable();
}

void SQLITE::deleteAllSalesManTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("DELETE FROM salesMan;");

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nDeleted All Data from salesMan Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;
	SQLITE::salesMan_ID = 0;
	
	sqlite3_close(DB);
}

void SQLITE::deleteAllOrdersTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("DELETE FROM Orders;");

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nDeleted all datas from Order Table successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::deleteAllCustomerTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("DELETE FROM Customer;");

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nDelete all datas from Customer Table successfully!\n");
	}
	

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::unOrderName()
{
	int rc;
	sqlite3* DB;
	char* errMsg;
	string sqlCommand("SELECT cust_name AS unOrderName, grade FROM Customer WHERE customer_id NOT IN (SELECT DISTINCT customer_id FROM Orders) LIMIT 1;");

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callbackTitle, 0, &errMsg);
	sqlCommand = "SELECT cust_name AS unOrderName, grade FROM Customer WHERE customer_id NOT IN (SELECT DISTINCT customer_id FROM Orders);";
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::updateSalesManTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;

	cout << "Enter salesManID for Update: ", cin >> this->saleManTab.salesMan_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// Update Data
	cout << "Update for salesMan has ID: " << this->saleManTab.salesMan_ID << endl;
	cout << "Name: ", getline(cin,this->saleManTab.Name);
	cout << "City: ", getline(cin, this->saleManTab.City);
	cout << "Commission: ", cin >> this->saleManTab.Commission;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	string sqlCommand("UPDATE salesMan SET Name = '" + this->saleManTab.Name + "', City = '" + this->saleManTab.City
		+ "', Commission = " + to_string(this->saleManTab.Commission)
		+ " WHERE salesMan_ID = " + to_string(this->saleManTab.salesMan_ID) + ";");

	//cout << "sql Command: " << sqlCommand << endl;

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nUpdate Data successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::updateOrdersTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;

	cout << "Enter Order_No for Update: ", cin >> this->orderTab.Order_No;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	// Update Data
	cout << "Update for The Order has Order_No: " << this->orderTab.Order_No << endl;
	cout << "Purch_AMT: ", cin >> this->orderTab.Purch_AMT;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Order_Date: ", getline(cin, this->orderTab.Order_Date);
	cout << "Customer_ID: ", cin >> this->orderTab.Customer_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "salesMan_ID: ", cin >> this->orderTab.salesMan_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	string sqlCommand("UPDATE Orders SET Purch_AMT = " + to_string(this->orderTab.Purch_AMT) + ", Order_Date = '" + this->orderTab.Order_Date
		+ "', Customer_ID = " + to_string(this->orderTab.Customer_ID)
		+ ", salesMan_ID = " + to_string(this->orderTab.salesMan_ID)
		+ " WHERE Order_No = " + to_string(this->orderTab.Order_No) + ";");

	//cout << "sql Command: " << sqlCommand << endl;

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nUpdate Data successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}

void SQLITE::updateCustomerTable()
{
	int rc;
	sqlite3* DB;
	char* errMsg;

	cout << "Enter Customer_ID for Update: ", cin >> this->cusTab.Customer_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// Update Data
	cout << "Update for salesMan has ID: " << this->cusTab.Customer_ID << endl;
	cout << "Cust_Name: ", getline(cin, this->cusTab.Cust_Name);
	cout << "City: ", getline(cin, this->cusTab.City);
	cout << "Grade: ", cin >>  this->cusTab.Grade;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "salesMan_ID: ", cin >> this->cusTab.salesMan_ID;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	string sqlCommand("UPDATE Customer SET Cust_Name = '" + this->cusTab.Cust_Name
		+ "', Grade = " + to_string(this->cusTab.Grade)
		+ ", City = '" + this->cusTab.City
		+ "', salesMan_ID = " + to_string(this->cusTab.salesMan_ID)
		+ " WHERE Customer_ID = " + to_string(this->cusTab.Customer_ID) + ";");

	//cout << "sql Command: " << sqlCommand << endl;

	rc = sqlite3_open(pathToDB.c_str(), &DB);
	rc = sqlite3_exec(DB, sqlCommand.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "\nError: %s!\n", errMsg);
	}
	else
	{
		fprintf(stdout, "\nUpdate Data successfully!\n");
	}

	delete errMsg;
	errMsg = nullptr;

	sqlite3_close(DB);
}


/*	STATIC METHOD FOR WORING STATIC VARIABLES */
void SQLITE::salesManIdIncreasing()
{
	salesMan_ID++;
}

void SQLITE::ordersNoIncreasing()
{
	Order_No++;
}

void SQLITE::customerIdIncreasing()
{
	Customer_ID++;
}

int SQLITE::getSalesManID() noexcept
{
	return salesMan_ID;
}

int SQLITE::getOrderNo()noexcept
{
	return Order_No;
}

int SQLITE::getCusID() noexcept
{
	return Customer_ID;
}



/* ****************************** CALL-BACK FUNCTION ******************************* */
// FOR get values of Cols
int callback(void* data, int argc, char** argv, char** azColName) {
	int i;	
	cout << "|";
	for (i = 0; i < argc; i++) {
		cout << left << setw(20) << setfill(' ') << argv[i] << "|";
 	}
	printf("\n");
	return 0;
}

// For get Name Of Cols
int callbackTitle(void* data, int argc, char** argv, char** azColName) {
	int i;
	int count = 0;
	cout << "|";
	for (i = 0; i < argc; i++) {
		cout << left << setw(20) << setfill(' ') << azColName[i] << "|";
		count++;
	}
	cout << endl;
	cout << "  ";
	for (int idex = 0; idex < count * 20; idex++)
	{
		cout << "-";
	}
	printf("\n");
	return 0;
}

// cin.ignore(numeric_limits<streamsize>::max(),'\n');

