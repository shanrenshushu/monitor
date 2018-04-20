#include "stdafx.h"
#include "Manager.h"



Manager* Manager::Instance = NULL;
Manager::Manager(void)
{
	if (!Instance)
	{
		Instance = this;
	}
	else
	{
		ASSERT(0);
	}
	Init();

}


Manager::~Manager(void)
{
	UnInit();
}

void Manager::Init()
{
}
void Manager::UnInit()
{
}

eSafeLevel Manager::GetSafeLevel( eDeviceType type,const sRecordInfo &info )
{
	if (type == eTiny)
	{
		float x = info.XData;
		float z = info.ZData;
		if ( x>=DEVIC_TYPE1_WARN1 || z>=DEVIC_TYPE1_WARN1)
		{
			return eBad;
		}
		else if (x>=DEVIC_TYPE1_WARN2 || z>=DEVIC_TYPE1_WARN2)
		{
			return eUnsafe;
		}
		else if (x>=DEVIC_TYPE1_WARN3 || z>=DEVIC_TYPE1_WARN3)
		{
			return eSafe;
		}
		else
		{
			return eGood;
		}
	}
	if (type == eSmall)
	{
		float x = info.XData;
		float z = info.ZData;
		if ( x>=DEVIC_TYPE2_WARN1 || z>=DEVIC_TYPE2_WARN1)
		{
			return eBad;
		}
		else if (x>=DEVIC_TYPE2_WARN2 || z>=DEVIC_TYPE2_WARN2)
		{
			return eUnsafe;
		}
		else if (x>=DEVIC_TYPE2_WARN3 || z>=DEVIC_TYPE2_WARN3)
		{
			return eSafe;
		}
		else
		{
			return eGood;
		}
	}
	if (type == eDeviceType::eMedium)
	{
		float x = info.XData;
		float z = info.ZData;
		if ( x>=DEVIC_TYPE3_WARN1 || z>=DEVIC_TYPE3_WARN1)
		{
			return eBad;
		}
		else if (x>=DEVIC_TYPE3_WARN2 || z>=DEVIC_TYPE3_WARN2)
		{
			return eUnsafe;
		}
		else if (x>=DEVIC_TYPE3_WARN3 || z>=DEVIC_TYPE3_WARN3)
		{
			return eSafe;
		}
		else
		{
			return eGood;
		}
	}
	if (type == eDeviceType::eBig)
	{
		float x = info.XData;
		float z = info.ZData;
		if ( x>=DEVIC_TYPE4_WARN1 || z>=DEVIC_TYPE4_WARN1)
		{
			return eBad;
		}
		else if (x>=DEVIC_TYPE4_WARN2 || z>=DEVIC_TYPE4_WARN2)
		{
			return eUnsafe;
		}
		else if (x>=DEVIC_TYPE4_WARN3 || z>=DEVIC_TYPE4_WARN3)
		{
			return eSafe;
		}
		else
		{
			return eGood;
		}
	}
	return eNolink;
}

void Manager::CheckTable()
{
	mRecordDB.CheckTable();
	mDeviceDB.CheckTable();
	mPersonDB.CheckTable();
	mComDB.CheckTable();
}

union my_union{  
	unsigned short s;  
	unsigned char c;  
};  
bool Manager::CheckSystemBig()
{  
	unsigned short s = 0x1234;  
	unsigned char *c = (unsigned char *)&s;
	return (*c == 0x12);  
}

