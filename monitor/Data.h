#pragma once
#include <windows.h>
#include <string.h>
#include <vector>

#include "common.h"
using namespace std;

//#define DEVIC_TYPE1 L"微型设备"
//#define DEVIC_TYPE2 L"小型设备"
//#define DEVIC_TYPE3 L"中型设备"
//#define DEVIC_TYPE4 L"大型设备"

#define DEVIC_TYPE1_WARN1 7.10f
#define DEVIC_TYPE1_WARN2 2.80f
#define DEVIC_TYPE1_WARN3 1.12f
//#define DEVIC_TYPE1_WARN4 0.28f

#define DEVIC_TYPE2_WARN1 11.20f
#define DEVIC_TYPE2_WARN2 4.50f
#define DEVIC_TYPE2_WARN3 1.80f
//#define DEVIC_TYPE2_WARN4 0.28f

#define DEVIC_TYPE3_WARN1 18.00f
#define DEVIC_TYPE3_WARN2 7.10f
#define DEVIC_TYPE3_WARN3 2.80f
//#define DEVIC_TYPE3_WARN4 0.28f

#define DEVIC_TYPE4_WARN1 28.0f
#define DEVIC_TYPE4_WARN2 11.20f
#define DEVIC_TYPE4_WARN3 4.50f
//#define DEVIC_TYPE4_WARN4 0.28f

enum eSafeLevel
{
	eGood=0,
	eSafe,
	eUnsafe,
	eBad,
	eNolink
};
enum eDeviceType
{
	eTiny=0,
	eSmall,
	eMedium,
	eBig
};


typedef struct __sDeviceInfo
{
	unsigned int ID;
	unsigned int deviceID;
	wstring name;
	wstring com;
	wstring type;
	int		baud;
	float	XWarning;
	float	ZWarning;
	float	TWarning;
	unsigned char address;
	wstring mark;
	__sDeviceInfo()
	{
		ID=0;
		deviceID=0;
		baud=9600;
		XWarning=0;
		ZWarning=0;
		TWarning=0;
		address=0;
	}
	virtual string GetTableName()const{return string("device");}
	void FillFields(MYSQL_ROW row,int numRows)
	{
		int index =0;
		if( row[index++]!=NULL )
		{
			ID = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			deviceID = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			name = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			com = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			type = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			baud = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			XWarning = atof(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			ZWarning = atof(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			TWarning = atof(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			address = (unsigned char )atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			mark = A2U(row[index-1]);
			if (index >= numRows) return;
		}
	}
	void to_sql_insert(string &out_sql_insert)const
	{
		string vals;
		vals = U2A(uint2str(deviceID));
		vals += ",'";
		vals += U2A(name);
		vals += "','";
		vals += U2A(com);
		vals += "','";
		vals += U2A(type);
		vals += "',";
		vals += U2A(int2str(baud));
		vals += ",";
		vals += U2A(float2str(XWarning));
		vals += ",";
		vals += U2A(float2str(ZWarning));
		vals += ",";
		vals += U2A(float2str(TWarning));
		vals += ",";
		vals += U2A(uint2str(address));
		vals += ",'";
		vals += U2A(mark);
		vals += "'";
		string temp="INSERT INTO " + GetTableName();
		out_sql_insert = temp + " ( deviceID,name,com,type,baud,XWarning,ZWarning,TWarning,address,mark ) VALUES (" 
			+ vals + ");";
	}
	void to_sql_update(string &out_sql_update)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"name='%s',com='%s',type='%s',baud=%u,XWarning=%.2f,ZWarning=%.2f,TWarning=%.2f,address=%d,mark='%s'  where deviceID=%u"
			,U2A(name).c_str(),U2A(com).c_str(),U2A(type).c_str(),baud,XWarning,ZWarning,TWarning,address,U2A(mark).c_str(),deviceID);

		string temp="UPDATE " + GetTableName() + " set ";
		out_sql_update = temp + buf ;
	}
	void to_sql_delete(int deviceid,string &out_sql_delete)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"DELETE from %s where deviceID=%u",GetTableName().c_str(),deviceid);

		out_sql_delete=buf;
	}

}sDeviceInfo,*psDeviceInfo;

///////////////////////////
typedef struct __sRecordInfo
{
	unsigned int ID;
	unsigned int deviceID;
	wstring	createTime;
	unsigned createTimeNum;
	float	XData;
	float	ZData;
	float	TData;
	__sRecordInfo()
	{
		ID=0;
		deviceID=0;
		XData=0;
		ZData=0;
		TData=0;
	}
	string GetTableName()const
	{
		char buf[40]={0};
		SYSTEMTIME st;
		GetLocalTime(&st);
		sprintf_s(buf, "%04d%02d", st.wYear, st.wMonth);
		string name = "record";
		name+=buf;
		return name;
	}
	void FillFields(MYSQL_ROW row,int numRows)
	{
		int index =0;
		if( row[index++]!=NULL )
		{
			ID = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			deviceID = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			createTime = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			createTimeNum = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			XData = atof(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			ZData = atof(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			TData = atof(row[index-1]);
			if (index >= numRows) return;
		}
	}
	void to_sql_insert(string &out_sql_insert)const
	{
		string vals;
		vals = U2A(uint2str(deviceID));
		vals += ",'";
		vals += U2A(createTime);
		vals += "',";
		vals += U2A(uint2str(createTimeNum));
		vals += ",";
		vals += U2A(float2str(XData));
		vals += ",";
		vals += U2A(float2str(ZData));
		vals += ",";
		vals += U2A(float2str(TData));

		string temp="INSERT INTO " + GetTableName();
		out_sql_insert = temp + " ( deviceID,createTime,createTimeNum,XData,ZData,TData) VALUES (" 
			+ vals + ");";
	}
	void to_sql_update(string &out_sql_update)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"createTime='%s',createTimeNum=%u,XData=%.2f,ZData=%.2f,TData=%.2f  where deviceID=%u"
			,U2A(createTime).c_str(),createTimeNum,XData,ZData,TData,deviceID);

		string temp="UPDATE " + GetTableName() + " set ";
		out_sql_update = temp + buf ;
	}
	void to_sql_delete(int deviceid,string &out_sql_delete)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"DELETE from %s where deviceID=%u",GetTableName().c_str(),deviceid);

		out_sql_delete=buf;
	}

}sRecordInfo,*psRecordInfo;

///////////////////////////
typedef struct __sPersonInfo
{
	unsigned int ID;
	wstring	jobNum;
	wstring	name;
	wstring phone;
	int checked;
	__sPersonInfo()
	{
		ID=0;
		checked=0;
	}
	string GetTableName()const{return string("person");}
	void FillFields(MYSQL_ROW row,int numRows)
	{
		int index =0;
		if( row[index++]!=NULL )
		{
			ID = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			jobNum = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			name = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			phone = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			checked = atoi(row[index-1]);
			if (index >= numRows) return;
		}
	}
	void to_sql_insert(string &out_sql_insert)const
	{
		string vals;
		vals += "'";
		vals += U2A(jobNum);
		vals += "','";
		vals += U2A(name);
		vals += "','";
		vals += U2A(phone);
		vals += "',";
		vals += U2A(uint2str(checked));

		string temp="INSERT INTO " + GetTableName();
		out_sql_insert = temp + " ( jobNum,name,phone,checked) VALUES (" 
			+ vals + ");";
	}
	void to_sql_update(string &out_sql_update)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"name='%s',phone='%s',checked=%d  where jobNum='%s'"
			,U2A(name).c_str(),U2A(phone).c_str(),checked,U2A(jobNum).c_str());

		string temp="UPDATE " + GetTableName() + " set ";
		out_sql_update = temp + buf ;
	}
	void to_sql_delete(string jobnum,string &out_sql_delete)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"DELETE from %s where jobNum=%u",GetTableName().c_str(),jobnum.c_str());

		out_sql_delete=buf;
	}

}sPersonInfo,*psPersonInfo;

///////////////////////////
typedef struct __sComInfo
{
	unsigned int ID;
	wstring	name;
	wstring mark;
	__sComInfo()
	{
		ID=0;
	}
	string GetTableName()const{return string("com");}
	void FillFields(MYSQL_ROW row,int numRows)
	{
		int index =0;
		if( row[index++]!=NULL )
		{
			ID = atoi(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			name = A2U(row[index-1]);
			if (index >= numRows) return;
		}
		if( row[index++]!=NULL )
		{
			mark = A2U(row[index-1]);
			if (index >= numRows) return;
		}
	}
	void to_sql_insert(string &out_sql_insert)const
	{
		string vals;
		vals += "'";
		vals += U2A(name);
		vals += "','";
		vals += U2A(mark);
		vals += "'";

		string temp="INSERT INTO " + GetTableName();
		out_sql_insert = temp + " ( name,mark) VALUES (" 
			+ vals + ");";
	}
	void to_sql_update(string &out_sql_update)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"mark='%s'  where name='%s'"
			,U2A(mark).c_str(),U2A(name).c_str());

		string temp="UPDATE " + GetTableName() + " set ";
		out_sql_update = temp + buf ;
	}
	void to_sql_delete(string com,string &out_sql_delete)const
	{
		char buf[1024]={0};
		sprintf_s(buf,1024,"DELETE from %s where com='%s'",GetTableName().c_str(),com.c_str());

		out_sql_delete=buf;
	}

}sComInfo,*psComInfo;

struct sDataMsg
{
	int state;
	sRecordInfo data;
	sDataMsg()
	{
		state=0;
	}
};