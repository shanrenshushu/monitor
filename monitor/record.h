#pragma once
#include "stdafx.h"
#include "Data.h"
#include "DbMysql.h"

class cRecordDB: public DbMysql<sRecordInfo> 
{  
public:  
	cRecordDB();
	~cRecordDB();

	virtual void CheckTable();

	vector<int> GetAllDeviceID();
	bool GetAllDevice(vector<sRecordInfo> &ve);
	int GetMaxDeviceID();
	sRecordInfo GetRecordByDeviceID(int deviceid);
	bool SearchDataByTime(vector<sRecordInfo*> &inArr,const string &begintime,const string &endtime,int deviceid);
};  