#include <string>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>
using namespace std;

//	删除单个指针
#ifndef DEL_PTR
#define DEL_PTR(p)              if (p != NULL) { delete p; p = NULL; }
#endif

//	删除数组指针
#ifndef DEL_PTR_ARRAY
#define DEL_PTR_ARRAY(p)        if (p != NULL) { delete [] p; p = NULL; }
#endif

extern string U2A(const wstring& str);//Unicode字符转Ascii字符</span>  

extern string GetSqlString(const wstring &str);

extern wstring A2U(const string& str);//Ascii字符转  

extern wstring int2str(int i);
extern wstring uint2str(unsigned int i);
extern wstring float2str(float i);

extern string U2Utf(const wstring& wstrUnicode);//Unicode转utf8    

extern wstring Utf2U(const string &str);//utf8转Unicode  
//分割字符串  
extern bool SplitString(const wstring& strSource,const wstring& strFlag, vector<wstring>& paramList); 
//URL编码  
extern string UrlEncode(const string& strSrc);
//URL解码  
extern string UrlDecode(const string& strSrc);
//替换字符串

extern wstring StrReplaceW(const wstring& strContent, const wstring& strTag, const wstring& strReplace);  

extern string StrReplaceA( const string& strContent, const string& strTag, const string& strReplace );

extern unsigned char* CRC16(unsigned char *bufData,unsigned short buflen);
extern void AUTOCRC16(unsigned char *bufData,unsigned short buflen);//自动计算crc16补充bufdata最后两个字节，buflen是bugdata总的长度
extern bool CRC_Check(unsigned char *bufData,unsigned short buflen);

extern string GetCurTime();

extern time_t GetTodayBeginTime();
extern time_t GetTodayNowTime();
extern time_t GetTodayEndTime();

extern DWORD TO_RGB(DWORD color);

extern time_t ToTimeStamp(string strTime);

extern wstring GetTimeInterval(string strTime);

extern std::string Format(const char *fmt,...);
extern std::wstring Format(const wchar_t *fmt,...);

extern void Log(unsigned char* inData,int len);
extern vector<wstring> GetAllCom();

extern int wstrToInt(const wstring& str);

extern void AdjustBuf(unsigned char* buf,unsigned short buflen,unsigned char pos,unsigned char data);