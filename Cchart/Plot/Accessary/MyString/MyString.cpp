/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2011 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

//#include "stdafx.h"
#include "MyString.h"
#include "sys/stat.h"
////////////////////////////////////////////////////////////////////////////////////////////////
//
//定义一些操作字符串的函数

void string_tolower ( tstring & str ) 
{
	transform(str.begin(),str.end(),str.begin(),tolower);
}

void string_toupper ( tstring & str ) 
{
	transform(str.begin(),str.end(),str.begin(),toupper);
}

tstring&   replace_all(tstring&   str,const   tstring&   old_value,const   tstring&   new_value)   
{   
    while(true)
	{
		tstring::size_type   pos(0);   
        if(   (pos=str.find(old_value))!=tstring::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}

tstring&   replace_all_distinct(tstring&   str,const   tstring&   old_value,const   tstring&   new_value)   
{   
    for(tstring::size_type   pos(0);   pos!=tstring::npos;   pos+=new_value.length())
	{
        if(   (pos=str.find(old_value,pos))!=tstring::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}

tstring::size_type find_first_nospace(tstring str)
{
	tstring::size_type i=0;
	do
	{
		if(i==str.size()-1)
		{
			if(isspace(str[i]))
				return tstring::npos;
			else
				return str.size()-1;
		}
		if(isspace(str[i]))
		{
			i++;
			continue;
		}
		else
		{
			return i;
		}
	}while(true);
}

tstring::size_type rfind_first_nospace(tstring str)
{
	tstring::size_type i=str.size()-1;
	do
	{
		if(i==0)
		{
			if(isspace(str[i]))
				return tstring::npos;
			else
				return 0;
		}
		if(isspace(str[i]))
		{
			i--;
			continue;
		}
		else
		{
			return i;
		}
	}while(true);
}

void trimleft(tstring &str)
{
	tstring::size_type idx=find_first_nospace(str);
	if(idx!=tstring::npos && idx<str.size() )str.erase(0,idx);
}

void trimright(tstring &str)
{
	tstring::size_type idx=rfind_first_nospace(str);
	if(idx!=tstring::npos && idx<str.size() )str.erase(idx+1);
}

void trim(tstring& str, const tstring drop)
{
	// trim right
	str.erase(str.find_last_not_of(drop)+1);
	// trim left
	str.erase(0,str.find_first_not_of(drop));
} 

void trimleft(tstring& str, const tstring drop)
{
	// trim left
	str.erase(0,str.find_first_not_of(drop));
} 

void trimright(tstring& str, const tstring drop)
{
	// trim right
	str.erase(str.find_last_not_of(drop)+1);
}

void	trimspace(tstring &str)
{
	tstring::size_type pos1, pos2;
	pos1 = find_first_nospace(str);
	pos2 = rfind_first_nospace(str);

	str = str.substr(pos1, pos2);
}

//对字符串操作，模拟读文件的一行，默认以\n为换行的符号
tstring readline(const tstring& buffer,tstring::size_type &curpos, const tstring symbol)
{
	tstring ret;
	tstring::size_type pos;
	pos=buffer.find_first_of(symbol,curpos);
	if(pos==tstring::npos)
	{
		curpos=0;
		return _TEXT("");
	}
	else
	{
		ret=buffer.substr(curpos,pos-curpos);
		curpos=pos+1;
		return ret;
	}
}

string mywcs2mbs(wstring wstr)
{
	int len = wcslen(wstr.c_str());
	if(len<=0)return "";

	char *str = new char[len+1];
	
	int nbyte = wcstombs(str, wstr.c_str(), len+1);
	if(nbyte<=0)
	{
		delete []str;
		return "";
	}
	string val = str;

	delete []str;
	return val;
}

long get_file_size( const TCHAR* filename )
{
	struct _stat f_stat; 
	if( _tstat( filename, &f_stat ) == -1 )
	{
		return -1; 
	}
	return (long)f_stat.st_size; 
}

long get_file_size( FILE* fp)
{
	long curpos,filesize;
	curpos=ftell(fp);
	fseek(fp,0L,SEEK_END);
    filesize=ftell(fp); 
	fseek(fp,curpos,SEEK_SET);
	return filesize;
}