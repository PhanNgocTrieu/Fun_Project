#pragma once

#ifndef __MSQLLITE_H__
#define __MSQLLITE_H__

#include <iostream>
using namespace std;

// Arrange types of data for optimizing memory

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


class Tables
{
protected:
	
public:
	// Constructor and Destructor
	virtual ~Tables() {};

	// Table
	virtual void createTable() = 0;
	virtual void droptable() = 0;

	// Values
	virtual void insertValues() = 0;
	virtual void deleteValues() = 0;
	virtual void updateValues() = 0;
	virtual void selectValues() = 0;

};



class mSQLITE
{
};


#endif // !__MSQLite_H__


