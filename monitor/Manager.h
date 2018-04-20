#pragma once

#include "DbMySql.h"
#include "Data.h"
#include "record.h"
#include "device.h"
#include "person.h"
#include "com.h"


class Manager
{
public:
	static Manager* Instance;

	static Manager* CreateInstance() { Instance = new Manager; return Instance;}
	static void ReleaseInstance() { if(Instance) { delete Instance; Instance = NULL; } }
	static Manager* GetInstance() {
		if(Instance==NULL) return CreateInstance();
		return Instance;
	}

public:
	Manager(void);
	~Manager(void);
	
	void Init();
	void UnInit();
	void CheckTable();

	cDeviceDB*   GetDeviceDB(){return &mDeviceDB;}
	cRecordDB* GetRecordDB(){return &mRecordDB;}
	cPersonDB*   GetPersonDB(){return &mPersonDB;}
	cComDB* GetComDB(){return &mComDB;}
	void SetMainWnd(HWND wnd){mHwnd = wnd;}
	HWND GetMainWnd(){return mHwnd;}

	eSafeLevel GetSafeLevel(eDeviceType type,const sRecordInfo &info);
	bool CheckSystemBig();//返回系统是不是大端

private:

	cDeviceDB   mDeviceDB;
	cRecordDB	mRecordDB;
	cPersonDB	mPersonDB;
	cComDB		mComDB;
	HWND mHwnd;
};
#define  MAN Manager::GetInstance()