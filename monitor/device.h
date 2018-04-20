#pragma once
#include "stdafx.h"
#include "Data.h"
#include "DbMysql.h"

class cDeviceDB: public DbMysql<sDeviceInfo> 
{  
public:  
	cDeviceDB();
	~cDeviceDB();

	virtual void CheckTable();

	vector<int> GetAllDeviceID();
	bool GetAllDevice(vector<sDeviceInfo> &ve);
	bool GetAllEffectDevice(vector<sDeviceInfo> &ve);
	
	int GetMaxDeviceID();
	sDeviceInfo GetDeviceByDeviceID(int deviceid);
};  