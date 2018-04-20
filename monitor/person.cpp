#include "StdAfx.h"
#include "person.h"

cPersonDB::cPersonDB()
{

}

cPersonDB::~cPersonDB()
{

}

void cPersonDB::CheckTable()
{
	MYSQL mysql;
	InitDb(mysql);
	sPersonInfo data;
	string sql=Format("CREATE TABLE IF NOT EXISTS `%s` (\
		`ID` int(11) NOT NULL AUTO_INCREMENT,\
		`jobNum` varchar(20) CHARACTER SET gbk NOT NULL,\
		`name` varchar(20) CHARACTER SET gbk DEFAULT NULL,\
		`phone` varchar(20) CHARACTER SET gbk DEFAULT NULL,\
		`checked` int(11) NOT NULL,\
		PRIMARY KEY (`ID`)\
		) ENGINE=InnoDB DEFAULT CHARSET=latin1;",data.GetTableName().c_str());
	int nRet = mysql_query(&mysql, sql.c_str());
	if (nRet != 0)
	{
		MessageBox(0,A2U(mysql_error(&mysql)).c_str(),L"CREATE error",MB_OK);
	}
	mysql_close(&mysql);
}

bool cPersonDB::GetAllPerson(vector<sPersonInfo> &ve)
{
	vector<int> vec;
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	sPersonInfo data;
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

bool cPersonDB::SelectPersonByID( int id )
{
	sPersonInfo data;
	string sql = Format("UPDATE %s SET checked=1 where ID=%d ",data.GetTableName().c_str(),id);
	Execue(sql);
	return true;
}
