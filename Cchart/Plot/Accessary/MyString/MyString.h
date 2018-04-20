#ifndef __MYSTRING_32167_H_122333444455555__
#define __MYSTRING_32167_H_122333444455555__

#pragma warning(disable:4786)

#include <tchar.h>

#include <cctype>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
typedef basic_istringstream<TCHAR> tistringstream;
typedef basic_ostringstream<TCHAR> tostringstream;
typedef basic_ifstream<TCHAR, char_traits<TCHAR> > tifstream;
typedef basic_ofstream<TCHAR, char_traits<TCHAR> > tofstream;
typedef basic_ios<TCHAR, char_traits<TCHAR> > tios;

//string functions
void string_tolower ( tstring &str ); 
void string_toupper ( tstring &str );
tstring& replace_all(tstring &str,const tstring &old_value,const tstring &new_value);
tstring& replace_all_distinct(tstring &str,const tstring &old_value,const tstring &new_value);
tstring::size_type find_first_nospace(tstring str);
tstring::size_type rfind_first_nospace(tstring str);
void trimleft(tstring &str);
void trimright(tstring &str);
void trim(tstring &str, const tstring drop = _TEXT(" "));
void trimleft(tstring &str, const tstring drop);
void trimright(tstring &str, const tstring drop);
void trimspace(tstring &str);
tstring readline(const tstring &buffer,tstring::size_type &curpos, const tstring symbol=_TEXT("\n"));

string mywcs2mbs(wstring wstr);

long get_file_size( const TCHAR *filename );
long get_file_size( FILE *fp);

#endif