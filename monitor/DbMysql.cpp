#include "stdafx.h"
//#include "DbMysql.h"
//#include "common.h"
//
//template<typename _Elem>
//DbMysql<_Elem>::DbMysql(void)
//{
//}
//
//template<typename _Elem>
//DbMysql<_Elem>::~DbMysql(void)
//{
//}
//template<typename _Elem>
//void DbMysql<_Elem>::InitDb(MYSQL& mysql)
//{
//	mysql_init(&mysql);
//	if(!mysql_real_connect(&mysql, "localhost", "root", "123456", "test", 3306, NULL, 0))
//	{
//		MessageBox(0,L"¡¥Ω” ß∞‹",0,MB_OK);
//	}
//	else
//	{
//		mysql_set_character_set(&mysql,"gbk");
//	}
//}
//template<typename _Elem>
//void DbMysql<_Elem>::InsertRecord(const _Elem& data)
//{
//	string strSql;
//	data.to_sql_insert(strSql);
//
//	MYSQL mysql;
//	InitDb(mysql);
//
//	int nRet = mysql_query(&mysql, strSql.c_str());
//	if (nRet != 0)
//	{
//		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"insert error",MB_OK);
//	}
//	else
//		int count=	mysql_affected_rows(&mysql);
//	mysql_close(&mysql);
//
//}
//template<typename _Elem>
//vector<_Elem> DbMysql<_Elem>::QueryRecord(const string& sqlQuery)
//{
//	vector<_Elem> vec_dbData;
//	MYSQL mysql;
//	MYSQL_RES* res;
//	MYSQL_ROW  row;
//
//	//sqlQuery = "select * from board_record";
//	int nRet = 0;
//
//	InitDb(mysql);
//	nRet = mysql_real_query(&mysql,sqlQuery.c_str(),strlen(sqlQuery.c_str()));  
//	if(nRet)  
//	{  
//		printf("failed to query:%s\n",mysql_error(&mysql));  
//  
//	} 
//	else 
//	{
//		res = mysql_store_result(&mysql);  
//		if( mysql_num_rows(res) != NULL )
//		{
//			int numRows = mysql_num_rows(res);
//			while( row = mysql_fetch_row(res) )
//			{
//				_Elem tmpData;
//				int count = mysql_num_fields(res);
//				tmpData.FillFields(row,count);
//			
//				vec_dbData.push_back(tmpData);
//			}
//		}	
//		mysql_free_result(res);
//	} 
//
//	mysql_close(&mysql);
//	return vec_dbData;
//}
//
//template<typename _Elem>
//void DbMysql<_Elem>::UpdateRecord(const string &sql_update)
//{
//	MYSQL mysql;
//	InitDb(mysql);
//
//	if (0!= mysql_real_query(&mysql, sql_update.c_str(), sql_update.length()))
//	{
//		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"Update error",MB_OK);
//	}
//}
//
//template<typename _Elem>
//vector<int> DbMysql<_Elem>::GetAllDeviceID()
//{
//	vector<int> vec;
//	MYSQL mysql;
//	MYSQL_RES* res;
//	MYSQL_ROW  row;
//
//	int nRet = 0;
//
//	InitDb(mysql);
//	_Elem data;
//	string sql = Format("select deviceID from %s group by deviceID",data.GetTableName().c_str());
//	nRet = mysql_real_query(&mysql,sql.c_str(),strlen(sql.c_str()));  
//	if(nRet)  
//	{  
//		//LogInfo("failed to query:%s\n",mysql_error(&mysql));  
//		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"query error",MB_OK);
//	} 
//	else 
//	{
//		res = mysql_store_result(&mysql);  
//		if( mysql_num_rows(res) != NULL )
//		{
//			int numRows = mysql_num_rows(res);
//			while( row = mysql_fetch_row(res) )
//			{
//				sSettingInfo tmpData;
//				int count = mysql_num_fields(res);
//				int id = atoi(row[0]);
//				vec.push_back(id);
//			}
//		}	
//		mysql_free_result(res);
//	} 
//
//	mysql_close(&mysql);
//	return vec;
//}
//
//template<typename _Elem>
//_Elem DbMysql<_Elem>::GetRecordByDeviceID(int deviceid)
//{
//	vector<int> vec;
//	MYSQL mysql;
//	MYSQL_RES* res;
//	MYSQL_ROW  row;
//	_Elem data;
//	int nRet = 0;
//
//	InitDb(mysql);
//	string sql = Format("select * from %s where deviceID=%d order by ID desc limit 1",data.GetTableName().c_str(), deviceid);
//	nRet = mysql_real_query(&mysql,sql.c_str(),strlen(sql.c_str()));  
//	if(nRet)  
//	{  
//		//LogInfo("failed to query:%s\n",mysql_error(&mysql));  
//		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"query error",MB_OK);
//	} 
//	else 
//	{
//		res = mysql_store_result(&mysql);  
//		if( mysql_num_rows(res) != NULL )
//		{
//			int numRows = mysql_num_rows(res);
//			while( row = mysql_fetch_row(res) )
//			{
//				int count = mysql_num_fields(res);
//				data.FillFields(row,count);
//				break;;
//			}
//		}	
//		mysql_free_result(res);
//	} 
//
//	mysql_close(&mysql);
//	return data;
//}
//
