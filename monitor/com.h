#pragma once
#include "stdafx.h"
#include "Data.h"
#include "DbMysql.h"

class cComDB: public DbMysql<sComInfo> 
{  
public:  
	cComDB();
	~cComDB();

	virtual void CheckTable();

	bool GetAllCom(vector<sComInfo> &ve);
};  