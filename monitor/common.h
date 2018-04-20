#include <string>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>
using namespace std;

//	ɾ������ָ��
#ifndef DEL_PTR
#define DEL_PTR(p)              if (p != NULL) { delete p; p = NULL; }
#endif

//	ɾ������ָ��
#ifndef DEL_PTR_ARRAY
#define DEL_PTR_ARRAY(p)        if (p != NULL) { delete [] p; p = NULL; }
#endif

extern string U2A(const wstring& str);//Unicode�ַ�תAscii�ַ�</span>  

extern string GetSqlString(const wstring &str);

extern wstring A2U(const string& str);//Ascii�ַ�ת  

extern wstring int2str(int i);
extern wstring uint2str(unsigned int i);
extern wstring float2str(float i);

extern string U2Utf(const wstring& wstrUnicode);//Unicodeתutf8    

extern wstring Utf2U(const string &str);//utf8תUnicode  
//�ָ��ַ���  
extern bool SplitString(const wstring& strSource,const wstring& strFlag, vector<wstring>& paramList); 
//URL����  
extern string UrlEncode(const string& strSrc);
//URL����  
extern string UrlDecode(const string& strSrc);
//�滻�ַ���

extern wstring StrReplaceW(const wstring& strContent, const wstring& strTag, const wstring& strReplace);  

extern string StrReplaceA( const string& strContent, const string& strTag, const string& strReplace );

extern unsigned char* CRC16(unsigned char *bufData,unsigned short buflen);
extern void AUTOCRC16(unsigned char *bufData,unsigned short buflen);//�Զ�����crc16����bufdata��������ֽڣ�buflen��bugdata�ܵĳ���
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