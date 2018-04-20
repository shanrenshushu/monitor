#include "StdAfx.h"
#include "device.h"

cDeviceDB::cDeviceDB()
{

}

cDeviceDB::~cDeviceDB()
{

}

void cDeviceDB::CheckTable()
{
	MYSQL mysql;
	InitDb(mysql);
	sDeviceInfo data;
	string sql=Format("CREATE TABLE IF NOT EXISTS `%s` (\
		`ID` int(10) unsigned NOT NULL AUTO_INCREMENT,\
		`deviceID` int(10) unsigned NOT NULL,\
		`name` varchar(20) CHARACTER SET gbk DEFAULT NULL,\
		`com` varchar(6) CHARACTER SET gbk NOT NULL,\
		`type` varchar(20) CHARACTER SET gbk NOT NULL,\
		`baud` int(11) NOT NULL,\
		`XWarning` float NOT NULL,\
		`ZWarning` float NOT NULL,\
		`TWarning` float NOT NULL,\
		`address` tinyint(3) unsigned NOT NULL,\
		`mark` varchar(64) CHARACTER SET gbk DEFAULT NULL,\
		PRIMARY KEY (`ID`),\
		KEY `indexfordeviceid` (`deviceID`) USING HASH\
		) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=latin1;",data.GetTableName().c_str());
	int nRet = mysql_query(&mysql, sql.c_str());
	if (nRet != 0)
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"CREATE error",MB_OK);
	}
	mysql_close(&mysql);
}
vector<int> cDeviceDB::GetAllDeviceID()
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;

	int nRet = 0;

	InitDb(mysql);
	sDeviceInfo data;
	string sql = Format("select deviceID from %s order by deviceID",data.GetTableName().c_str());
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
				sDeviceInfo tmpData;
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

bool cDeviceDB::GetAllDevice(vector<sDeviceInfo> &ve)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sDeviceInfo data;
	int nRet = 0;

	InitDb(mysql);
	string sql = Format("select * from %s  order by ID desc ",data.GetTableName().c_str());
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

bool cDeviceDB::GetAllEffectDevice(vector<sDeviceInfo> &ve)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sDeviceInfo data;
	int nRet = 0;

	InitDb(mysql);
	string sql = Format("select * from %s where address <> 0 and com is not null order by ID desc ",data.GetTableName().c_str());
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

sDeviceInfo cDeviceDB::GetDeviceByDeviceID(int deviceid)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sDeviceInfo data;
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

int cDeviceDB::GetMaxDeviceID()
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sDeviceInfo data;
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

	mysql_close(&mysql);
	return deviceID;
}
