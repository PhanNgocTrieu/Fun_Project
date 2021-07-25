#pragma once

#ifndef __SQLITE_H__
#define __SQLITE_H__

#include <iostream>
using namespace std;

// Arrange types of data for optimizing memory
typedef struct saleMans {
	string		Name;
	string		City;
	double		Commission;
	int			salesMan_ID;
	saleMans() : Name{ "" }, City{ "" }, Commission{ 0.0 }, salesMan_ID{ -1 } {}
} SALEMANS;


// Arrange types of data for optimizing memory
typedef struct Orders {
	string		Order_Date;
	double		Purch_AMT;
	int			Order_No;
	int			Customer_ID;
	int			salesMan_ID;
	Orders() : Order_Date{ "" }, Purch_AMT{ 0.0 }, Order_No{ -1 }, Customer_ID{ -1 }, salesMan_ID{ -1 } {}
} ORDERS;

typedef struct Customers {
	string		Cust_Name;
	string		City;
	int			Grade;
	int			Customer_ID;
	int			salesMan_ID;
	Customers() : Cust_Name{ "" }, City{ "" }, Grade{ -1 }, Customer_ID{ -1 }, salesMan_ID{ -1 }{}
} CUSTOMERS;

class SQLITE
{
private:

	SALEMANS saleManTab;
	ORDERS orderTab;
	CUSTOMERS cusTab;
	string pathToDB;

	// Static ID Variables
	static int	salesMan_ID;
	static int	Order_No;
	static int	Customer_ID;

public:
	

	// Constructors and Destructor
	SQLITE();
	SQLITE(const string& pathToDB);
	SQLITE(const SQLITE& _other);
	~SQLITE();

	// Handling Database Methods - SOLID
	void createDatabase(const string& pathToDB);
	void createSalesManTable();
	void createOrderTable();
	void createCustomerTable();
	void createThreeTables();  // For running example

	// INSERT QUERY METHOD
	void insertSalesManTable();
	void insertOrderTable();
	void insertCustomerTable();
	void insertAutomatically(); // For running example

	// SELECT QUERY METHOD
	void listSalesManTable();
	void listOrdersTable();
	void listCustomerTable();
	void listAllOfTable();

	// DROP QUERY METHOD
	void dropTables();
	void resetTables();

	// DELETE QUERY METHOD
	void deleteAllSalesManTable();
	void deleteAllOrdersTable();
	void deleteAllCustomerTable();
	

	// ANOTHER METHOD
	void unOrderName();


	// UPDATE QUERY METHOD
	void updateSalesManTable();
	void updateOrdersTable();
	void updateCustomerTable();


	// Static methods for using Static Variables:
	static void salesManIdIncreasing();
	static void ordersNoIncreasing();
	static void customerIdIncreasing();
	static int getSalesManID() noexcept;
	static int getOrderNo() noexcept;
	static int getCusID() noexcept;
};


static int callback(void* data, int argc, char** argv, char** azColName);
static int callbackTitle(void* data, int argc, char** argv, char** azColName);


#endif // !__SQLITE_H__


