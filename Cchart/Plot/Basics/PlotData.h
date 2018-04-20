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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is a templated class 

#ifndef __PLOTDATA_H_122333444455555__
#define __PLOTDATA_H_122333444455555__

//#include "Origin.h"
#include "../Accessary/Deelx/deelx.h"
#include "../Accessary/MyString/MyString.h"
#include "PlotImpl.h"
#include <vector>

typedef struct stDataPoint2D
{
	double x,y;
}DataPoint2D;

typedef struct stDataPoint3D
{
	double x,y,z;
}DataPoint3D;

typedef vector<double> MyVData1D;
typedef vector<DataPoint2D> MyVData2D;
typedef vector<DataPoint3D> MyVData3D;

void GetDataPoint(tstring aline, double &result);
void GetDataPoint(tstring aline, DataPoint2D &result);
void GetDataPoint(tstring aline, DataPoint3D &result);

void GetDataLine(double datapoint, tstring &result);
void GetDataLine(DataPoint2D datapoint, tstring &result);
void GetDataLine(DataPoint3D datapoint, tstring &result);

////////////////////////////////////////////////////////////////////////////////////////////

template<typename DataT>
class CPlotData
{
public:
	CPlotData();
	virtual ~CPlotData();

protected:
	vector<DataT>		m_vData;

	tstring				m_strTitle;
	tstring				m_strInfo;//Draw at the first point
	bool				m_bInfoDraw;
	int					m_nInfoFontsize;

	int					m_nDataLevel;

public:
	void				CopySettings(const CPlotData *plotdata);
	void				CopyAll(const CPlotData *plotdata);

	inline	const vector<DataT>		&GetDataConst() const {return m_vData;}
	inline	vector<DataT>			&GetData() {return m_vData;}
	inline	int						GetDataSize() {return m_vData.size();}

	void				DeleteData( void );
	inline tstring		GetTitle(){if(m_strTitle==_TEXT("") )return _TEXT("Untitled");	else return m_strTitle;	};
	inline void			SetTitle(tstring title){m_strTitle=title;};
	inline tstring		GetInfo(){return m_strInfo;};
	inline void			SetInfo(tstring info){m_strInfo=info;};
	inline bool			IsInfoDraw(){return m_bInfoDraw;}
	inline void			SetInfoDraw(bool bDraw){m_bInfoDraw=bDraw;}
	inline int			GetInfoFontsize(){return m_nInfoFontsize;}
	inline void			SetInfoFontsize(int fontsize){if(fontsize>4)m_nInfoFontsize=fontsize;else m_nInfoFontsize=4;}
	inline int			GetDataLevel(){return m_nDataLevel;}
	inline void			SetDataLevel(int level){m_nDataLevel=level;}

	bool				SetData( vector<DataT> vData );

	bool				ReadFromBuff(tstring buff);
	bool				ReadFromFile(tstring filename);
	bool				WriteToFile(tofstream &ofs);
	bool				WriteToFile(tstring filename);
	bool				AppendToFile(tstring filename);
};

template<typename DataT>
CPlotData<DataT>::CPlotData()
{
	m_strTitle = _TEXT("Untitled");
	m_strInfo = _TEXT("");
	m_bInfoDraw = false;
	m_nInfoFontsize = 12;
	
	m_nDataLevel = -1;
}

template<typename DataT>
CPlotData<DataT>::~CPlotData()
{
	DeleteData();
}

template<typename DataT>
void CPlotData<DataT>::CopySettings(const CPlotData* plotdata)
{
	m_strTitle = plotdata->m_strTitle;
	m_strInfo = plotdata->m_strInfo;
	m_bInfoDraw = plotdata->m_bInfoDraw;
	m_nInfoFontsize = plotdata->m_nInfoFontsize;
	m_nDataLevel = plotdata->m_nDataLevel;
}

template<typename DataT>
void CPlotData<DataT>::CopyAll(const CPlotData* plotdata)
{
	CopySettings(plotdata);
	m_vData = plotdata->m_vData;
}

// Delete the data set
template<typename DataT>
void CPlotData<DataT>::DeleteData(void)
{
	// Check if valid data. If so, delete it
	m_strTitle = _TEXT("");
	m_vData.clear();
}

template<typename DataT>
bool CPlotData<DataT>::SetData(vector<DataT> vData)
{
	if (vData.size() <= 0)
		return false;
	m_vData.clear();
	m_vData = vData;
	return true;
}

template<typename DataT>
bool CPlotData<DataT>::ReadFromBuff(tstring buff)
{
	int	num = sizeof(DataT)/sizeof(double);
	if(num<=0 || num>3)return false;
	
	tstring	pattern;
	if(num==1)
	{
		pattern = _TEXT("^\\s*-?\\d+(.\\d*)([eE]\\d*)*\\s*$");
	}
	else if(num==2)
	{
		pattern = _TEXT("^\\s*(-?\\d+(.\\d*)([eE]\\d*)*\\s*){2}$");
	}
	else if(num==3)
	{
		pattern = _TEXT("^\\s*(-?\\d+(.\\d*)([eE]\\d*)*\\s*){3}$");
	}

	tstring aline;
	CRegexpT <TCHAR> dataline(pattern.c_str());
	MatchResult rst1;
	int match1;
	
	DataT datapoint;

	vector<DataT> vData;
	tstring value;
	tstring::size_type curpos;
	
	curpos=0;
	do
	{
		aline=readline(buff,curpos);
		rst1=dataline.MatchExact(aline.c_str());
		match1=rst1.IsMatched();
		if(match1)
		{
			GetDataPoint(aline, datapoint);
			vData.push_back(datapoint);
		}
	}while(curpos>0);

	if(vData.size()<=0)
	{
		return false;
	}
	else
	{
		m_vData.clear();
		m_vData = vData;
		return true;
	}
}

template<typename DataT>
bool CPlotData<DataT>::ReadFromFile(tstring filename)
{
	tifstream ifs;
#if defined(_UNICODE) || defined(UNICODE)
	ifs.open(mywcs2mbs(filename).c_str());
#else
	ifs.open(filename.c_str());
#endif
	if(!ifs.is_open())return false;
	
	int filesize=get_file_size(filename.c_str());
	TCHAR *pBuff=new TCHAR[filesize+1];
	memset(pBuff,0,(filesize+1)*sizeof(TCHAR));
	ifs.read(pBuff,filesize);
	tstring content=pBuff;
	delete []pBuff;
	ifs.close();

	return ReadFromBuff(content);
}

template<typename DataT>
bool	CPlotData<DataT>::WriteToFile(tofstream &ofs)
{
	if(!ofs.is_open() || m_vData.size()<=0)
		return false;
	
	tstring aline,content;
	int i;
	
	for(i=0; i<(int)m_vData.size(); i++)
	{
		GetDataLine(m_vData[i], aline);
		content += aline;
	}

	ofs.write(content.c_str(), content.length());
	return true;
}

template<typename DataT>
bool	CPlotData<DataT>::WriteToFile(tstring filename)
{
	if(m_vData.size()<=0)
		return false;
	tofstream ofs;
#if defined(_UNICODE) || defined(UNICODE)
	ofs.open(mywcs2mbs(filename).c_str());
#else
	ofs.open(filename.c_str());
#endif
	if(!ofs.is_open())return false;
	bool ret = WriteToFile(ofs);
	ofs.close();

	return ret;
}

template<typename DataT>
bool	CPlotData<DataT>::AppendToFile(tstring filename)
{
	if(m_vData.size()<=0)
		return false;

	tofstream ofs;
	tofstream ofs;
#if defined(_UNICODE) || defined(UNICODE)
	ofs.open(mywcs2mbs(filename).c_str(), tios::app);
#else
	ofs.open(filename.c_str(),tios::app);
#endif
	if(!ofs::is_open())return false;
	bool ret = WriteToFile(ofs);
	ofs.close();
	return ret;
}

#endif