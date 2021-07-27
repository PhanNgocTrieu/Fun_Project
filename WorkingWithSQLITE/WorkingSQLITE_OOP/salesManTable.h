#pragma once

#ifndef __SALESMANTABLE_H__
#define __SALESMANTABLE_H__

#include "mSQLITE.h"

typedef struct salesMan {
	string		Name;
	string		City;
	double		Commission;
	int			salesMan_ID;
	salesMan() : Name{ "" }, City{ "" }, Commission{ 0.0 }, salesMan_ID{ -1 } {}
} SALEMANS;


class salesManTable : public Tables
{
private:
	SALEMANS mTable;

public:
	~salesManTable() {};
	void createTable();
	void droptable();
	void insertValues();
	void deleteValues();
	void updateValues();
	void selectValues();

	static int callbackValues(void* data, int argc, char** argv, char** azColName);
	static int callbackColName(void* data, int argc, char** argv, char** azColName);
};

#endif // !__SALESMANTABLE_H__


