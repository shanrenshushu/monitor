#include "StdAfx.h"
#include "record.h"

cRecordDB::cRecordDB()
{

}

cRecordDB::~cRecordDB()
{

}

void cRecordDB::CheckTable()
{
	MYSQL mysql;
	InitDb(mysql);
	//sRecordInfo data;

	char buf[40]={0};
	SYSTEMTIME st;
	GetLocalTime(&st);
	for (int i=st.wMonth;i<=12;i++)
	{
		sprintf_s(buf, "%04d%02d", st.wYear, i);
		string name = "record";
		name+=buf;
		string sql=Format("CREATE TABLE IF NOT EXISTS`%s` (\
						  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,\
						  `deviceID` int(10) unsigned NOT NULL,\
						  `createTime` datetime NOT NULL,\
						  `createTimeNum` int(10) unsigned DEFAULT NULL,\
						  `XData` float NOT NULL,\
						  `ZData` float NOT NULL,\
						  `TData` float NOT NULL,\
						  PRIMARY KEY (`ID`),\
						  KEY `indexfordeviceid` (`deviceID`) USING HASH,\
						  KEY `indexforcreatetime` (`createTime`) USING HASH\
						  ) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;",name.c_str());
		int nRet = mysql_query(&mysql, sql.c_str());
		if (nRet != 0)
		{
			MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"CREATE error",MB_OK);
		}
	}
	for (int i=1;i<=12;i++)
	{
		sprintf_s(buf, "%04d%02d", st.wYear+1, i);
		string name = "record";
		name+=buf;
		string sql=Format("CREATE TABLE IF NOT EXISTS`%s` (\
						  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,\
						  `deviceID` int(10) unsigned NOT NULL,\
						  `createTime` datetime NOT NULL,\
						  `createTimeNum` int(10) unsigned DEFAULT NULL,\
						  `XData` float NOT NULL,\
						  `ZData` float NOT NULL,\
						  `TData` float NOT NULL,\
						  PRIMARY KEY (`ID`),\
						  KEY `indexfordeviceid` (`deviceID`) USING HASH,\
						  KEY `indexforcreatetime` (`createTime`) USING HASH\
						  ) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;",name.c_str());
		int nRet = mysql_query(&mysql, sql.c_str());
		if (nRet != 0)
		{
			MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"CREATE error",MB_OK);
		}
	}
	
	mysql_close(&mysql);
}
vector<int> cRecordDB::GetAllDeviceID()
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;

	int nRet = 0;

	InitDb(mysql);
	sRecordInfo data;
	string sql = Format("select deviceID from %s group by deviceID",data.GetTableName().c_str());
	nRet = mysql_real_query(&mysql,sql.c_str(),strlen(sql.c_str()));  
	if(nRet)  
	{  
		//LogInfo("failed to query:%s\n",mysql_error(&mysql));  
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"query error",MB_OK);
	} 
	else 
	{
		res = mysql_store_result(&mysql);  
		if( mysql_num_rows(res) != NULL )
		{
			int numRows = mysql_num_rows(res);
			while( row = mysql_fetch_row(res) )
			{
				sRecordInfo tmpData;
				int count = mysql_num_fields(res);
				int id = atoi(row[0]);
				vec.push_back(id);
			}
		}	
		mysql_free_result(res);
	} 

	mysql_close(&mysql);
	return vec;
}

bool cRecordDB::GetAllDevice(vector<sRecordInfo> &ve)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sRecordInfo data;
	int nRet = 0;

	InitDb(mysql);
	string sql = Format("select * from %s order by ID desc ",data.GetTableName().c_str());
	nRet = mysql_real_query(&mysql,sql.c_str(),strlen(sql.c_str()));  
	if(nRet)  
	{  
		//LogInfo("failed to query:%s\n",mysql_error(&mysql));  
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"query error",MB_OK);
	} 
	else 
	{
		res = mysql_store_result(&mysql);  
		if( mysql_num_rows(res) != NULL )
		{
			int numRows = mysql_num_rows(res);
			while( row = mysql_fetch_row(res) )
			{
				int count = mysql_num_fields(res);
				data.FillFields(row,count);
				ve.push_back(data);
			}
		}	
		mysql_free_result(res);
	} 

	mysql_close(&mysql);
	return true;
}

sRecordInfo cRecordDB::GetRecordByDeviceID(int deviceid)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sRecordInfo data;
	int nRet = 0;

	InitDb(mysql);
	string sql = Format("select * from %s where deviceID=%d order by ID desc limit 1",data.GetTableName().c_str(), deviceid);
	nRet = mysql_real_query(&mysql,sql.c_str(),strlen(sql.c_str()));  
	if(nRet)  
	{  
		//LogInfo("failed to query:%s\n",mysql_error(&mysql));  
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"query error",MB_OK);
	} 
	else 
	{
		res = mysql_store_result(&mysql);  
		if( mysql_num_rows(res) != NULL )
		{
			int numRows = mysql_num_rows(res);
			while( row = mysql_fetch_row(res) )
			{
				int count = mysql_num_fields(res);
				data.FillFields(row,count);
				break;;
			}
		}	
		mysql_free_result(res);
	} 

	mysql_close(&mysql);
	return data;
}

int cRecordDB::GetMaxDeviceID()
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sRecordInfo data;
	int nRet = 0;
	int deviceID = 0;

	InitDb(mysql);
	string sql = Format("select max(deviceID) from %s ",data.GetTableName().c_str());
	nRet = mysql_real_query(&mysql,sql.c_str(),strlen(sql.c_str()));  
	if(nRet)  
	{  
		//LogInfo("failed to query:%s\n",mysql_error(&mysql));  
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"query error",MB_OK);
	} 
	else 
	{
		res = mysql_store_result(&mysql);  
		if( mysql_num_rows(res) != NULL )
		{
			int numRows = mysql_num_rows(res);
			while( row = mysql_fetch_row(res) )
			{
				if (row[0])
				{
					deviceID = atoi(row[0]);
				}
				break;;
			}
		}	
		mysql_free_result(res);
	} 

	//mysql_close(&mysql);
	return deviceID;
}

bool cRecordDB::SearchDataByTime( vector<sRecordInfo*> &inArr,const string &begintime,const string &endtime,int deviceid )
{
	sRecordInfo data;
	string sql=Format("select * from %s where deviceID=%d and createTime between '%s' and '%s' order by createTime asc ",data.GetTableName().c_str(),deviceid,begintime.c_str(),endtime.c_str());
	return Query(inArr,sql);
}