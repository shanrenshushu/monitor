#pragma once
#include "Data.h"

#include "stdafx.h"
#include "DbMysql.h"
#include "common.h"

template<typename _Elem>
class DbMysql
{
public:
	DbMysql(void);
	virtual ~DbMysql(void);

	virtual void CheckTable()=0;

	void Insert(const _Elem& data);
	bool Query(vector<_Elem*> &inArr,const string& sqlQuery);
	void Update(const string &sql_update);
	void Update(vector<_Elem*> &inArr);
	void Update(const _Elem &inArr);
	void Execue(const string& sqlQuery);
	void Delete(int id);
	
protected:
	void InitDb(MYSQL& mysql);
	//MYSQL mysql;
};


template<typename _Elem>
DbMysql<_Elem>::DbMysql(void)
{
	//InitDb(mysql);
}

template<typename _Elem>
DbMysql<_Elem>::~DbMysql(void)
{
	//mysql_close(&mysql);
}
template<typename _Elem>
void DbMysql<_Elem>::InitDb(MYSQL& mysql)
{
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "localhost", "root", "123456", "test", 3306, NULL, 0))
	{
		//MessageBox(0,L"¡¥Ω” ß∞‹",0,MB_OK);
	}
	else
	{
		mysql_set_character_set(&mysql,"gbk");
	}
}
template<typename _Elem>
void DbMysql<_Elem>::Insert(const _Elem& data)
{
	string strSql;
	data.to_sql_insert(strSql);

	MYSQL mysql;
	InitDb(mysql);

	int nRet = mysql_query(&mysql, strSql.c_str());
	if (nRet != 0)
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"insert error",MB_OK);
	}
	else
		int count=	mysql_affected_rows(&mysql);
	mysql_close(&mysql);

}
template<typename _Elem>
bool DbMysql<_Elem>::Query(vector<_Elem*> &inArr,const string& sqlQuery)
{
	vector<_Elem> vec_dbData;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;

	//sqlQuery = "select * from board_record";
	int nRet = 0;

	InitDb(mysql);
	nRet = mysql_real_query(&mysql,sqlQuery.c_str(),strlen(sqlQuery.c_str()));  
	if(nRet)  
	{  
		printf("failed to query:%s\n",mysql_error(&mysql));  
		return false;

	} 
	else 
	{
		res = mysql_store_result(&mysql);  
		if( mysql_num_rows(res) != NULL )
		{
			int numRows = mysql_num_rows(res);
			while( row = mysql_fetch_row(res) )
			{
				_Elem *tmpData = new _Elem;
				int count = mysql_num_fields(res);
				tmpData->FillFields(row,count);

				inArr.push_back(tmpData);
			}
		}	
		mysql_free_result(res);
	} 

	mysql_close(&mysql);
	return true;
}

template<typename _Elem>
void DbMysql<_Elem>::Update(const string &sql_update)
{
	MYSQL mysql;
	InitDb(mysql);

	if (0!= mysql_real_query(&mysql, sql_update.c_str(), sql_update.length()))
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"Update error",MB_OK);
	}
	mysql_close(&mysql);
}
template<typename _Elem>
void DbMysql<_Elem>::Update(vector<_Elem*> &inArr)
{
	MYSQL mysql;
	InitDb(mysql);
	for (int i=0;i<inArr.size();i++)
	{
		_Elem &data = inArr[i];
		string sql;
		data.to_sql_update(sql);
		if (0!= mysql_real_query(&mysql, sql.c_str(), sql.length()))
		{
			MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"Update error",MB_OK);
		}
	}
	mysql_close(&mysql);
}
template<typename _Elem>
void DbMysql<_Elem>::Update(const _Elem &data)
{
	MYSQL mysql;
	InitDb(mysql);

	string sql;
	data.to_sql_update(sql);
	if (0!= mysql_real_query(&mysql, sql.c_str(), sql.length()))
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"Update error",MB_OK);
	}
	mysql_close(&mysql);
}
template<typename _Elem>
void DbMysql<_Elem>::Execue(const string& sql)
{
	MYSQL mysql;
	InitDb(mysql);
	if (0!= mysql_real_query(&mysql, sql.c_str(), sql.length()))
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"Execue error",MB_OK);
	}
	mysql_close(&mysql);
}


template<typename _Elem>
void DbMysql<_Elem>::Delete(int id)
{
	MYSQL mysql;
	InitDb(mysql);
	_Elem e;
	string sql = Format("DELETE FROM %s WHERE ID=%d",e.GetTableName().c_str(),id);
	if (0!= mysql_real_query(&mysql, sql.c_str(), sql.length()))
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"Execue error",MB_OK);
	}
	mysql_close(&mysql);
}