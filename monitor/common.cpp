#include "stdafx.h"
#include "common.h"
#include <stdio.h>

typedef unsigned __int16    Int16;

//将小于16的数字转换为16进制字符（大写字母）
unsigned char ToHex(unsigned char x) {
	return  x > 9 ? x + 55 : x + 48;
}

//将16进制字符转换为数字
unsigned char FromHex(unsigned char x) {
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	return y;
}

string U2A(const wstring& str)//Unicode字符转Ascii字符</span>  
{  
	string strDes;  
	if ( str.empty() )  
		goto __end;  
	int nLen=::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);  
	if ( 0==nLen )  
		goto __end;  
	char* pBuffer=new char[nLen+1];  
	memset(pBuffer, 0, nLen+1);  
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), pBuffer, nLen, NULL, NULL);  
	pBuffer[nLen]='\0';  
	strDes.append(pBuffer);  
	delete[] pBuffer;  
__end:  
	return strDes;  
}  

wstring A2U(const string& str)//Ascii字符转  
{  
	wstring strDes;  
	if ( str.empty() )  
		goto __end;  
	int nLen=::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);  
	if ( 0==nLen )  
		goto __end;  
	wchar_t* pBuffer=new wchar_t[nLen+1];  
	memset(pBuffer, 0, nLen+1);  
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), pBuffer, nLen);  
	pBuffer[nLen]='\0';  
	strDes.append(pBuffer);  
	delete[] pBuffer;  
__end:  
	return strDes;  
}  

wstring int2str(int i)
{
	wchar_t buf[64];
	swprintf(buf,64,L"%d",i);

	return wstring(buf);
}

wstring uint2str(unsigned int i)
{
	wchar_t buf[64];
	swprintf(buf,64,L"%u",i);

	return wstring(buf);
}

wstring float2str(float i)
{
	wchar_t buf[64];
	swprintf(buf,64,L"%.2f",i);

	return wstring(buf);
}

string GetSqlString(const wstring &str)
{
	string strDest;
	strDest = " '" + U2A(str) + "' ";
	return strDest;
}

string U2Utf(const wstring& wstrUnicode)//Unicode转utf8    
{    
	wstring str = wstrUnicode;
	string strRet;  
	if( wstrUnicode.empty() )  
	return strRet;  
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstrUnicode.c_str(), -1, NULL, 0, NULL, NULL);    
	char* pBuffer=new char[nLen+1];  
	pBuffer[nLen] = '\0';  
	nLen = WideCharToMultiByte(CP_UTF8, 0, wstrUnicode.c_str(), -1, pBuffer, nLen, NULL, NULL);   
	strRet.append(pBuffer);  
	delete[] pBuffer;  
	return strRet; 
}  

wstring Utf2U(const string &str)//utf8转Unicode  
{  
	int u16Len = ::MultiByteToWideChar(CP_UTF8, NULL,str.c_str(),(int)str.size(), NULL, 0);  
	wchar_t* wstrBuf = new wchar_t[u16Len + 1];  
	::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(),(int)str.size(), wstrBuf, u16Len);  
	wstrBuf[u16Len] = L'\0';  
	wstring wStr;  
	wStr.assign(wstrBuf, u16Len);  
	delete [] wstrBuf;  
	return wStr;  
}  
//分割字符串  
bool SplitString(const wstring& strSource,const wstring& strFlag, vector<wstring>& paramList)  
{  
	if ( strSource.empty() || strFlag.empty() )  
		return false;  
	paramList.clear();  
	size_t nBeg = 0;  
	size_t nFind = strSource.find(strFlag, nBeg);  
	if ( nFind == std::wstring::npos )  
		paramList.push_back(strSource);  
	else  
	{  
		while ( true )  
		{  
			if ( nFind != nBeg )  
				paramList.push_back(strSource.substr(nBeg, nFind-nBeg));  
			nBeg = nFind + strFlag.size();  
			if ( nBeg == strSource.size() )  
				break;  
			nFind = strSource.find(strFlag, nBeg);  
			if ( nFind == std::wstring::npos )  
			{  
				paramList.push_back(wstring(strSource.begin()+nBeg, strSource.end()));  
				break;  
			}  
		}  
	}  
	return true;  
}  
//URL编码  
string UrlEncode(const string& strSrc)  
{  
	string strDes;  
	for ( size_t i=0; i<strSrc.size(); ++i )  
	{  
		BYTE ch=(BYTE)strSrc[i];  
		if ( isalnum(ch) || ch=='-' || ch=='_' || ch=='.' || ch=='~' )  
			strDes+=ch;  
		else if ( ch==' ' )  
			strDes+='+';  
		else  
		{  
			strDes+='%';  
			strDes+=ToHex( (ch>>4) );  
			strDes+=ToHex( ch%16 );  
		}  
	}  
	return strDes;  
}  
//URL解码  
string UrlDecode(const string& strSrc)  
{  
	string strDes;    
	for ( size_t i = 0; i < strSrc.size(); i++ )    
	{    
		BYTE ch=strSrc[i];  
		if (ch == '+')   
			strDes+=' ';    
		else if (ch == '%')    
		{    
			BYTE h = FromHex((unsigned char)strSrc[++i]);    
			BYTE l = FromHex((unsigned char)strSrc[++i]);    
			strDes += (h<<4) + l;    
		}    
		else strDes += ch;    
	}    
	return strDes;   
}  
//替换字符串  
wstring StrReplaceW(const wstring& strContent, const wstring& strTag, const wstring& strReplace)  
{  
	size_t nBegin=0, nFind=0;  
	nFind = strContent.find(strTag, nBegin);  
	if ( nFind == wstring::npos )  
		return strContent;  
	size_t nTagLen = strTag.size();  
	wstring strRet;  
	while ( true )  
	{  
		strRet.append(strContent.begin()+nBegin, strContent.begin()+nFind);  
		strRet.append(strReplace);  
		nBegin = nFind + nTagLen;  
		nFind = strContent.find(strTag, nBegin);  
		if ( nFind == wstring::npos )  
		{  
			strRet.append(strContent.begin()+nBegin, strContent.end());  
			break;  
		}  
	}  
	return strRet;  
}  

string StrReplaceA( const string& strContent, const string& strTag, const string& strReplace )  
{  
	size_t nBegin=0, nFind=0;  
	nFind = strContent.find(strTag, nBegin);  
	if ( nFind == string::npos )  
		return strContent;  
	size_t nTagLen = strTag.size();  
	string strRet;  
	while ( true )  
	{  
		strRet.append(strContent.begin()+nBegin, strContent.begin()+nFind);  
		strRet.append(strReplace);  
		nBegin = nFind + nTagLen;  
		nFind = strContent.find(strTag, nBegin);  
		if ( nFind == string::npos )  
		{  
			strRet.append(strContent.begin()+nBegin, strContent.end());  
			break;  
		}  
	}  
	return strRet;  
}  


//crc
unsigned char* CRC16(unsigned char *bufData,unsigned short buflen)
{
	int ret = 0;  
	Int16 CRC = 0xFFFF;  
	Int16 POLYNOMIAL = 0xA001;  
	unsigned char *res = new unsigned char[2];
	int i, j;  

	for (i = 0; i < buflen; i++)  
	{  
		CRC ^= (Int16)bufData[i];  
		for (j = 0; j < 8; j++)  
		{  
			if(CRC & 0x0001)  
			{  
				CRC >>= 1;  
				CRC ^= POLYNOMIAL;  
			}  
			else  
			{  
				CRC >>= 1;  
			}  
		}  
	} 
	res[0] = (unsigned char)(CRC & 0x00ff);
	res[1] = (unsigned char)((CRC >> 8) & 0x00ff); 
	return res;
}

void AUTOCRC16(unsigned char *bufData,unsigned short buflen)
{
	unsigned char *crc = CRC16(bufData,buflen-2);
	bufData[buflen-2] = crc[0];
	bufData[buflen-1] = crc[1];
	delete []crc;
}

std::string GetCurTime()
{
	char buf[40]={0};
	SYSTEMTIME st;

	GetLocalTime(&st);

	sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return buf;
}

time_t ToTimeStamp(string strTime)  
{  
	tm _tm;  
	int year, month, day, hour, minute,second;  

	sscanf_s(strTime.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);  
	_tm.tm_year  = year - 1900;  
	_tm.tm_mon   = month - 1;  
	_tm.tm_mday  = day;  
	_tm.tm_hour  = hour;  
	_tm.tm_min   = minute;  
	_tm.tm_sec   = second;  
	_tm.tm_isdst = 0;  
	time_t t = mktime(&_tm);  

	return t;  
}  

wstring GetTimeInterval(string strTime)
{
	time_t start = ToTimeStamp(strTime);
	time_t end;
	time(&end);
	double left = difftime(end, start);

	int h = (int)left / 3600;
	int m = ((int)left % 3600) / 60;
	int s = ((int)left % 3600) % 60;

	WCHAR tmp[50] = {0};
	swprintf_s(tmp, 50, L"%02d : %02d : %02d", h, m, s);
	wstring strRet(tmp);
	return strRet;
}

std::string Format(const char *fmt,...)
{
	std::string strResult="";
	if (NULL != fmt)
	{
		va_list marker;            
		va_start(marker, fmt);   
		size_t nLength = _vscprintf(fmt, marker) + 1;  
		std::vector<char> vBuffer(nLength, '\0');  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten>0)
		{
			strResult = &vBuffer[0];
		}            
		va_end(marker);   
	}
	return strResult; 
}

std::wstring Format(const wchar_t *fmt,...)
{
	std::wstring strResult=L"";
	if (NULL != fmt)
	{
		va_list marker;            
		va_start(marker, fmt);
		size_t nLength = _vscwprintf(fmt, marker) + 1; 
		std::vector<wchar_t> vBuffer(nLength, L'\0');
		int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker); 
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);
	}
	return strResult; 
}

extern time_t GetTodayBeginTime()
{
	char buf[40]={0};
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(buf, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	string beginTime =  buf;
	beginTime += " 00:00:00";
	tm tm_;  
	int year, month, day, hour, minute,second;  
	if (sscanf_s(beginTime.c_str(),"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6)
		return -1;
	tm_.tm_year  = year-1900;  
	tm_.tm_mon   = month-1;  
	tm_.tm_mday  = day;  
	tm_.tm_hour  = hour;  
	tm_.tm_min   = minute;  
	tm_.tm_sec   = second;  
	tm_.tm_isdst = 0;  

	time_t resTime = mktime(&tm_); //已经减了8个时

	return resTime;
}

extern time_t GetTodayNowTime()
{
	time_t resTime =time(0);
	return resTime;
}

extern time_t GetTodayEndTime()
{
	char buf[40]={0};
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(buf, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	string endTime = buf;
	endTime += " 23:59:59";
	tm tm_;  
	int year, month, day, hour, minute,second;  
	if (sscanf_s(endTime.c_str(),"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6)
		return -1;
	tm_.tm_year  = year-1900;  
	tm_.tm_mon   = month-1;  
	tm_.tm_mday  = day;  
	tm_.tm_hour  = hour;  
	tm_.tm_min   = minute;  
	tm_.tm_sec   = second;  
	tm_.tm_isdst = 0;  

	time_t resTime = mktime(&tm_); //已经减了8个时

	return resTime;
}

extern DWORD TO_RGB( DWORD color )
{
	DWORD r = color & 0x00ff0000;
	DWORD g = color & 0x0000ff00;
	DWORD b = color & 0x000000ff;

	 return (r>>16)|g|(b<<16);
}

extern void Log( unsigned char* inData,int len )
{
	wstring log;
	for (int i=0;i<len;i++)
	{
		log+= Format(L"%02X ",inData[i]);
	}
	LogInfo(log.c_str());
}

extern bool CRC_Check( unsigned char *bufData,unsigned short buflen )
{
	unsigned char *crc = CRC16(bufData,buflen-2);
	bool check = (bufData[buflen-2]== crc[0]) && (bufData[buflen-1]== crc[1]);
	delete []crc;
	return check;
}

extern vector<wstring> GetAllCom()
{
	int reg;
	HKEY hKey;  
	int rtn;  
	vector<wstring > ve;
	rtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm",  
		NULL, KEY_READ, &hKey); // 打开串口注册表  
	if( rtn == ERROR_SUCCESS)  
	{  
		int i=0;  
		wchar_t portName[256],commName[256];  
		DWORD dwLong,dwSize;
		
		while(1)  
		{  
			dwSize = sizeof(portName);  
			dwLong = dwSize;  
			rtn = RegEnumValue( hKey, i, portName, &dwLong,  
				NULL, NULL, (PUCHAR)commName, &dwSize );  

			if( rtn == ERROR_NO_MORE_ITEMS ) // 枚举串口  
			{
				break;  
			}
			ve.push_back(commName);
			i++;  
		}
		reg=i;
	}  
	RegCloseKey(hKey);  
	return ve;
}

extern int wstrToInt( const wstring& str )
{
	LPTSTR pstr = NULL;
	return _tcstol(str.c_str(), &pstr, 10);
}

void AdjustBuf(unsigned char* buf,unsigned short buflen,unsigned char pos,unsigned char data)
{
	if (pos>=buflen)
	{
		return;
	}
	buf[pos]=data;
	AUTOCRC16(buf,buflen);
}