// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// YZG.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

int ConvertToUnicode(const char* str ,wchar_t* out,UINT codepage /*= CP_ACP*/)
{
	size_t len = ::strlen(str);
	if (len == 0)
	{
		*out = _T('\0');
		return 0;
	}
	if (len >= STRMAXLONG)
	{
		*out = _T('\0');
		return 0;
	}
	int iLen = MultiByteToWideChar(codepage,0, str, len, NULL, 0);
	if (iLen > 0)
	{
		iLen = MultiByteToWideChar(codepage,0, str, -1, out, iLen+1);
		assert(iLen != 0);
	}
	else
	{
		iLen --;
	}
	return len;
}

int ConvertToAscii(const wchar_t* uniStr,char *out,UINT codepage /*= CP_ACP*/)
{
	size_t wideLen = ::wcslen(uniStr);
	if (wideLen == 0)
	{
		*out = '\0';
		return 0;
	}
	if (wideLen >= STRMAXLONG)
	{
		*out = '\0';
		return 0;
	}
	int iLen = WideCharToMultiByte(codepage, 0, uniStr, wideLen, out, 0, NULL, NULL);
	if (iLen > 0)
	{
		iLen = WideCharToMultiByte(codepage, 0, uniStr , -1, out, iLen+1, NULL, NULL);
	}
	else
	{
		iLen --;
	}
	return iLen;
}


string WStringToString(const wstring &str) {
	long l =	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, 0);
	LPSTR outstr = new char[l];
	memset(outstr,0,l*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, outstr, l, NULL, 0);
	string s = outstr;
	delete[] outstr;
	outstr = NULL;
	return s;
}


wstring StringToWString(const string &str) {
	long l = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	LPWSTR outstr = new wchar_t[l];
	memset(outstr,0,l*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, outstr, l);
	wstring s = outstr;
	delete[] outstr;
	outstr = NULL;
	return s;
}
