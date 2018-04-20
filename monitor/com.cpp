#include "StdAfx.h"
#include "com.h"

cComDB::cComDB()
{

}

cComDB::~cComDB()
{

}

void cComDB::CheckTable()
{
	MYSQL mysql;
	InitDb(mysql);
	sComInfo data;
	string sql=Format("CREATE TABLE IF NOT EXISTS `com` (\
		`ID` int(11) NOT NULL AUTO_INCREMENT,\
		`name` varchar(10) CHARACTER SET gbk NOT NULL,\
		`mark` varchar(20) CHARACTER SET gbk DEFAULT NULL,\
		PRIMARY KEY (`ID`)\
		) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;",data.GetTableName().c_str());
	int nRet = mysql_query(&mysql, sql.c_str());
	if (nRet != 0)
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"CREATE error",MB_OK);
	}
	mysql_close(&mysql);
}

bool cComDB::GetAllCom(vector<sComInfo> &ve)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sComInfo data;
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
