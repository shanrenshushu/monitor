#pragma once
#include "stdafx.h"
#include "Data.h"
#include "DbMysql.h"

class cPersonDB: public DbMysql<sPersonInfo> 
{  
public:  
	cPersonDB();
	~cPersonDB();

	virtual void CheckTable();

	bool GetAllPerson(vector<sPersonInfo> &ve);
	bool SelectPersonByID(int id);
};  