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

//#include "Stdafx.h"
#include "XYPlot.h"

#include <fstream>

/////////////////////////////////////////////////////////////////////////
// Class CPlot

CXYPlot::CXYPlot()
{
	SetDefaults();
}

CXYPlot::~CXYPlot()
{
	DeleteAll();
}

void	CXYPlot::SetDefaults()
{
	m_nCurDataID = 0;

	m_bMultiSelect = false;
	m_nShowDataSetInfoMode = kShowDataSetInfoEveryN;
	m_nShowDataSetInfoNum = 1;
	m_nShowDataSetInfoDataPointStep = 32;
	
	m_bKeepFormer = true;

	m_bAutoLineStyle = true;
	
	m_bAutoDataColor = true;
	m_nModeOfSetColors = kSetColorFromTable;
	m_crLineColor1		= RGB(255,0,0);
	m_crLineColor2		= RGB(255,255,0);

	m_fHitPrecision	= 128;

	for( int i = 0; i< GetDataSetCount(); i++ )
	{
		SetDefaults( i );
	}
}

// Set the defaults for all the variables in an XY Plot
void	CXYPlot::SetDefaults( int index )
{
	int			i = index;

	if( i < 0 || i >= GetDataSetCount() ) return;

#ifdef DEFAULT_WHITE_BACKGROUND
	m_vpXYDataInfo[i]->crDataColor = RGB(0,0,0);
	m_vpXYDataInfo[i]->crPlotColor = RGB(0,0,0);
#else
	m_vpXYDataInfo[i]->crDataColor = RGB(255,255,255);
	m_vpXYDataInfo[i]->crPlotColor = RGB(255,255,255);
#endif
	m_vpXYDataInfo[i]->bAutoColor = true;
	m_vpXYDataInfo[i]->bDataRangesSet = false;

	m_vpXYDataInfo[i]->nMarkerType = kXYMarkerNone;
	m_vpXYDataInfo[i]->nMarkerSize = 4;
	m_vpXYDataInfo[i]->bMarkerFill = false;
	m_vpXYDataInfo[i]->nMarkerFrequency = -10;
	m_vpXYDataInfo[i]->nPlotType = kXYPlotConnect;

	m_vpXYDataInfo[i]->nDataLineSize = 1;
	m_vpXYDataInfo[i]->nPlotLineSize = 1;

	m_vpXYDataInfo[i]->nDataLineStyle = PS_SOLID;
	m_vpXYDataInfo[i]->nPlotLineStyle = PS_SOLID;
	m_vpXYDataInfo[i]->bAutoLineStyle = false;

	m_vpXYDataInfo[i]->nDataFillMode = kDataFillClosed;
	m_vpXYDataInfo[i]->bDataFill = false;
	m_vpXYDataInfo[i]->nDataFillBrushType = kBrushSolid;
	m_vpXYDataInfo[i]->crDataFillColor = RGB(240,240,168);
	m_vpXYDataInfo[i]->nDataFillHatchStyle = HS_CROSS;
	m_vpXYDataInfo[i]->hDataFillPattern = NULL;

	m_vpXYDataInfo[i]->bVisible = true;
	m_vpXYDataInfo[i]->bSelected = false;
	m_vpXYDataInfo[i]->bLighted = false;

	m_vpXYDataInfo[i]->bReact = true;
}

// Copy the settings from one chart to another
void	CXYPlot::CopySettings( const CXYPlot *plot )
{
	m_nCurDataID = plot->m_nCurDataID;

	m_bMultiSelect					= plot->m_bMultiSelect;
	m_nShowDataSetInfoMode			= plot->m_nShowDataSetInfoMode;
	m_nShowDataSetInfoNum			= plot->m_nShowDataSetInfoNum;
	m_nShowDataSetInfoDataPointStep	= plot->m_nShowDataSetInfoDataPointStep;
	
	m_bKeepFormer					= plot->m_bKeepFormer;

	m_bAutoLineStyle				= plot->m_bAutoLineStyle;
	
	m_bAutoDataColor				= plot->m_bAutoDataColor;
	m_nModeOfSetColors				= plot->m_nModeOfSetColors;
	m_crLineColor1					= plot->m_crLineColor1;
	m_crLineColor2					= plot->m_crLineColor2;

	m_fHitPrecision					= plot->m_fHitPrecision;

	//if(plot->m_pLegend)m_pLegend->CopySettings(plot->m_pLegend);
}

void	CXYPlot::CopySettings( const CXYPlot *plot , int index)
{
	int i=index;
	if(i<0 || i>=GetDataSetCount())return;

	m_vpXYDataInfo[i]->nDataID = plot->m_vpXYDataInfo[i]->nDataID;
	m_vpXYDataInfo[i]->crDataColor = plot->m_vpXYDataInfo[i]->crDataColor;
	m_vpXYDataInfo[i]->crPlotColor = plot->m_vpXYDataInfo[i]->crPlotColor;
	m_vpXYDataInfo[i]->bAutoColor = plot->m_vpXYDataInfo[i]->bAutoColor;
	m_vpXYDataInfo[i]->bDataRangesSet = plot->m_vpXYDataInfo[i]->bDataRangesSet;
	
	m_vpXYDataInfo[i]->nMarkerType = plot->m_vpXYDataInfo[i]->nMarkerType;
	m_vpXYDataInfo[i]->nMarkerSize = plot->m_vpXYDataInfo[i]->nMarkerSize;
	m_vpXYDataInfo[i]->bMarkerFill = plot->m_vpXYDataInfo[i]->bMarkerFill;
	m_vpXYDataInfo[i]->nMarkerFrequency = plot->m_vpXYDataInfo[i]->nMarkerFrequency;
	m_vpXYDataInfo[i]->nPlotType = plot->m_vpXYDataInfo[i]->nPlotType;
	
	m_vpXYDataInfo[i]->nDataLineSize = plot->m_vpXYDataInfo[i]->nDataLineSize;
	m_vpXYDataInfo[i]->nDataLineStyle = plot->m_vpXYDataInfo[i]->nDataLineStyle;
	m_vpXYDataInfo[i]->nPlotLineStyle = plot->m_vpXYDataInfo[i]->nPlotLineStyle;
	m_vpXYDataInfo[i]->bAutoLineStyle = plot->m_vpXYDataInfo[i]->bAutoLineStyle;
	
	m_vpXYDataInfo[i]->bVisible = plot->m_vpXYDataInfo[i]->bVisible;
	m_vpXYDataInfo[i]->bSelected = plot->m_vpXYDataInfo[i]->bSelected;
	m_vpXYDataInfo[i]->bLighted = plot->m_vpXYDataInfo[i]->bLighted;
	
	m_vpXYDataInfo[i]->nDataFillMode = plot->m_vpXYDataInfo[i]->nDataFillMode;
	m_vpXYDataInfo[i]->bDataFill = plot->m_vpXYDataInfo[i]->bDataFill;
	m_vpXYDataInfo[i]->nDataFillBrushType = plot->m_vpXYDataInfo[i]->nDataFillBrushType;
	m_vpXYDataInfo[i]->crDataFillColor = plot->m_vpXYDataInfo[i]->crDataFillColor;
	m_vpXYDataInfo[i]->nDataFillHatchStyle = plot->m_vpXYDataInfo[i]->nDataFillHatchStyle;
	if(m_vpXYDataInfo[i]->hDataFillPattern)DeleteObject(m_vpXYDataInfo[i]->hDataFillPattern);
	m_vpXYDataInfo[i]->hDataFillPattern = plot->m_vpXYDataInfo[i]->hDataFillPattern;
	
	m_vpXYDataInfo[i]->bReact = plot->m_vpXYDataInfo[i]->bReact;

}

void	CXYPlot::CopyAll( const CXYPlot *plot )
{
	int	i;

	DeleteAllData();
	for( i=0; i<plot->GetDataSetCount(); i++ )
	{
		AddData(plot->GetAbsoluteData(i));
		m_vpXYDataInfo[i]->pDataSet->CopySettings(plot->m_vpXYDataInfo[i]->pDataSet);
	}

	for( i = 0; i < GetDataSetCount(); i++ )
	{
		CopySettings(plot, i);
	}
	
	CopySettings(plot);
}

// Deletes a data set, and moves all the settings
// around in their arrays
bool	CXYPlot::DeleteData( int dataID )
{
	int count=GetDataSetCount();
	for( int i = 0; i < count; i++ )
	{
		if( m_vpXYDataInfo[i]->nDataID == dataID )
		{
			// Delete this data set
			m_vpXYDataInfo[i]->pDataSet->DeleteData();
			// Free the object
			delete m_vpXYDataInfo[i]->pDataSet;
			// Free the handle of bitmap for brush pattern
			if(m_vpXYDataInfo[i]->hDataFillPattern)DeleteObject(m_vpXYDataInfo[i]->hDataFillPattern);
			// Delete this XYDataInfo
			delete m_vpXYDataInfo[i];
			m_vpXYDataInfo.erase(m_vpXYDataInfo.begin()+i);
			return true;
		}
	}

	return false;
}

// Deletes every data set
void	CXYPlot::DeleteAllData( void )
{
	bool		keepGoing = true;

	while( keepGoing == true && GetDataSetCount() > 0 )
	{
		keepGoing = DeleteData( GetDataID(0) );
	}
	//SetXAutoRange();
	//SetYAutoRange();
}

void	CXYPlot::DeleteAll( void )
{
	DeleteAllData();
}

int	CXYPlot::AddCurve( double *pX, double *pY, int nLen )
{
	if(!pX || !pY || nLen<=0)return -1;
	MyVData2D vData;
	DataPoint2D dp;
	for(int i=0;i<nLen;i++)
	{
		dp.x=pX[i];
		dp.y=pY[i];
		vData.push_back(dp);
	}
	return AddCurve(vData);
}

int CXYPlot::AddDataSet(MyVData2D vData,int charttype,COLORREF color,int markertype,int markerfreq)
{
	if(vData.size()<=0)return -1;

	if(!m_bKeepFormer)DeleteAllData();

	int		dataID = AddData( vData );
	if(dataID<0)return -1;

	SetPlotType(dataID,charttype);
	SetMarkerType(dataID,markertype);
	SetMarkerFrequency( dataID, markerfreq );
	SetDataColor(dataID,color);	

	return dataID;
}

int		CXYPlot::GetVisibleDataSetCount()
{
	int count = 0;
	for(int i=0; i<GetDataSetCount(); i++)
	{
		if(m_vpXYDataInfo[i]->bVisible)
			count++;
	}
	return count;
}

int		CXYPlot::GetIndexOfFirstVisibleDataSet()
{
	int index = -1;
	for(int i=0; i<GetDataSetCount(); i++)
	{
		if(m_vpXYDataInfo[i]->bVisible)
		{
			index = i;
			break;
		}
	}
	return index;
}
int		CXYPlot::GetIndexOfLastVisibleDataSet()
{
	int index = -1;
	for(int i=GetDataSetCount()-1; i>=0; i--)
	{
		if(m_vpXYDataInfo[i]->bVisible)
		{
			index = i;
			break;
		}
	}
	return index;
}

int		CXYPlot::GetIndexOfNextVisibleDataSet(int idxCurr)
{
	int index = -1;
	if(idxCurr >= GetDataSetCount())
	{
		return -1;
	}
	index = (idxCurr<0)?0:idxCurr+1;
	while(!IsVisible(GetDataID(index)))
	{
		index++;
		if(index >= GetDataSetCount())
		{
			index = -1;
			break;
		}
	}
	return index;	
}
int		CXYPlot::GetIndexOfPrevVisibleDataSet(int idxCurr)
{
	int index = -1;
	if(idxCurr >= GetDataSetCount())
	{
		return -1;
	}
	index = (idxCurr<0)?GetDataSetCount()-1:idxCurr-1;
	while(!IsVisible(GetDataID(index)))
	{
		index--;
		if(index <0)
		{
			index = -1;
			break;
		}
	}
	return index;
}

tstring		CXYPlot::GetDataTitle( int dataID )
{
	int i = GetIndex(dataID);
	if( i < 0 ) return _TEXT("");
	else return m_vpXYDataInfo[i]->pDataSet->GetTitle();
}

void		CXYPlot::SetDataTitle( int dataID, tstring title )
{
	int index = GetIndex( dataID );
	if( index < 0 || index >= GetDataSetCount() ) return;
	m_vpXYDataInfo[index]->pDataSet->SetTitle( title );
}

// Based on a dataID, get an index to the arrays
int		CXYPlot::GetIndex( int dataID ) const
{
	int			i;

	for( i = 0; i < GetDataSetCount(); i++ )
	{
		if( m_vpXYDataInfo[i]->nDataID == dataID )
		{
			return i;
		}
	}

	return -1;
}

const MyVData2D &CXYPlot::GetData( int dataID ) const
{
	int index=GetIndex(dataID);
	if(index<0)index=0;
	return m_vpXYDataInfo[index]->pDataSet->GetDataConst();
}

const MyVData2D &CXYPlot::GetAbsoluteData( int which ) const
{
	if(which<0 || which>GetDataSetCount())which=0;
	return m_vpXYDataInfo[which]->pDataSet->GetDataConst();
}

// Returns the data set requested, or NULL if invalid
CPlotData<DataPoint2D>	*CXYPlot::GetAbsoluteDataSet( int which )
{
	CPlotData<DataPoint2D>		*plotData;

	if( which < 0 || which >= GetDataSetCount() ) return NULL;

	plotData = m_vpXYDataInfo[which]->pDataSet;

	return plotData;
}
/*
tstring		CXYPlot::GetAbsoluteDataTitle( int which )
{
	CPlotData<DataPoint2D>		*plotData = GetAbsoluteDataSet( which );
	
	if(!plotData)
		return "";
	else
		return plotData->GetTitle();
}
*/
CPlotData<DataPoint2D>	*CXYPlot::GetDataSet( int dataID )
{
	int		i = GetIndex( dataID );

	if( i >= 0 ) return GetAbsoluteDataSet( i ); else return NULL;
}

// Sets up the data range variable for the
// specified data set
void	CXYPlot::GetDataRange( int dataID )
{
	int			i, j;
	double		minX, minY, maxX, maxY;
	CPlotData<DataPoint2D>	*dataSet;

	i = GetIndex( dataID );
	if( i < 0 ) return;

	if( m_vpXYDataInfo[i]->bDataRangesSet == false )
	{
		minX = 1e30; minY = 1e30;
		maxX = -1e30; maxY = -1e30;
		//m_vpXYDataInfo[i]->bDataRangesSet = true;
		
		dataSet = GetAbsoluteDataSet( i );
		
		// Just find min and max of x
		for( j = 0; j < dataSet->GetDataSize(); j++)
		{
			if( dataSet->GetData()[j].x < minX ) minX = dataSet->GetData()[j].x;
			if( dataSet->GetData()[j].x > maxX ) maxX = dataSet->GetData()[j].x;

			if( dataSet->GetData()[j].y < minY ) minY = dataSet->GetData()[j].y;
			if( dataSet->GetData()[j].y > maxY ) maxY = dataSet->GetData()[j].y;
		}
				
		m_vpXYDataInfo[i]->fXDataMin = minX;
		m_vpXYDataInfo[i]->fXDataMax = maxX;
		m_vpXYDataInfo[i]->fYDataMin = minY;
		m_vpXYDataInfo[i]->fYDataMax = maxY;
	}
}

// Gets all data ranges, and sets up the data range
// variable for them
void	CXYPlot::GetDataRanges( void )
{
	for( int i = 0; i < GetDataSetCount(); i++ )
	{
		GetDataRange( GetDataID(i) );
	}
}

void	CXYPlot::GetDataRanges(double xRange[2], double yRange[2])
{
	GetDataRanges();

	xRange[0]=1e30;
	xRange[1]=-1e30;
	yRange[0]=1e30;
	yRange[1]=-1e30;
	for( int i = 0; i < GetDataSetCount(); i++ )
	{
		if(m_vpXYDataInfo[i]->fXDataMin < xRange[0])xRange[0] = m_vpXYDataInfo[i]->fXDataMin;
		if(m_vpXYDataInfo[i]->fXDataMax > xRange[1])xRange[1] = m_vpXYDataInfo[i]->fXDataMax;
		if(m_vpXYDataInfo[i]->fYDataMin < yRange[0])yRange[0] = m_vpXYDataInfo[i]->fYDataMin;
		if(m_vpXYDataInfo[i]->fYDataMax > yRange[1])yRange[1] = m_vpXYDataInfo[i]->fYDataMax;
	}
}

///Data management

int	CXYPlot::AddData( MyVData2D vData )
{
	CPlotData<DataPoint2D> *dataSet = new CPlotData<DataPoint2D>;
	
	int returnVal = dataSet->SetData( vData );
	if( returnVal == false ) return -1;

	// Now add this to our typed pointer array
	XYDataInfo *pdi = new XYDataInfo;
	pdi->pDataSet = dataSet;
	pdi->nDataID = m_nCurDataID;
	m_vpXYDataInfo.push_back(pdi);
	SetDefaults( GetIndex( m_nCurDataID ) );
	GetDataRanges();
	
	//Rename the data name seqencely
	tostringstream ostr;
	ostr.fill('0');
	ostr.width(3);
	ostr<<GetIndex( m_nCurDataID );
	SetDataTitle(m_nCurDataID, GetDataTitle(m_nCurDataID)+ostr.str());

	m_nCurDataID++;
	return (m_nCurDataID -1);

}

int	CXYPlot::SetData( int dataID, MyVData2D vData )
{
	int index = GetIndex( dataID );
	if( index >= GetDataSetCount() || index < 0 ) return -1;

	if( m_vpXYDataInfo[index]->pDataSet->SetData( vData ) )
	{
		m_vpXYDataInfo[index]->bDataRangesSet = false;
		GetDataRange(dataID);
		return dataID;
	}
	else
		return -1;
}

bool	CXYPlot::SetDataModified( int dataID )
{
	if( dataID < 0 )
	{
		GetDataRanges();
		// all have been modified
	}
	else
	{
		m_vpXYDataInfo[GetIndex(dataID)]->bDataRangesSet=false;
		GetDataRange( dataID );
		// one has been modified
	}

	return true;
}

void	CXYPlot::SetPlotColors()
{
	int nNum=GetDataSetCount();
	short r,g,b;
	for( int i = 0; i < nNum; i++ )
	{
		//Set color and line size
		if(m_bAutoDataColor && m_vpXYDataInfo[i]->bAutoColor)
		{
			switch(m_nModeOfSetColors)
			{
			case kSetColorFromTwoColor:
				r= ( GetRValue(m_crLineColor1)*(nNum-i) + GetRValue(m_crLineColor2) )/nNum;
				g= ( GetGValue(m_crLineColor1)*(nNum-i) + GetGValue(m_crLineColor2) )/nNum;
				b= ( GetBValue(m_crLineColor1)*(nNum-i) + GetBValue(m_crLineColor2) )/nNum;
				m_vpXYDataInfo[i]->crPlotColor=RGB(r,g,b);
				break;
			default:
				m_vpXYDataInfo[i]->crPlotColor=colorTableForPlot[i%NUMOFCOLORS];
				break;
			}
		}
		else
		{
			m_vpXYDataInfo[i]->crPlotColor=m_vpXYDataInfo[i]->crDataColor;
		}
		m_vpXYDataInfo[i]->crPlotColor = CPlot::ChooseLineColor(m_vpXYDataInfo[i]->crPlotColor, m_vpXYDataInfo[i]->bLighted, m_vpXYDataInfo[i]->bSelected);
	}
}

void	CXYPlot::SetPlotLineStyles()
{
	for( int i = 0; i < GetDataSetCount(); i++ )
	{
		if(m_bAutoLineStyle && m_vpXYDataInfo[i]->bAutoLineStyle)
			m_vpXYDataInfo[i]->nPlotLineStyle=i%5;
		else
			m_vpXYDataInfo[i]->nPlotLineStyle=m_vpXYDataInfo[i]->nDataLineStyle;
	}
}

void	CXYPlot::SetPlotLineSizes()
{
	for( int i = 0; i < GetDataSetCount(); i++ )
	{
		m_vpXYDataInfo[i]->nPlotLineSize = CPlot::ChooseLineWidth(m_vpXYDataInfo[i]->nDataLineSize, m_vpXYDataInfo[i]->bLighted, m_vpXYDataInfo[i]->bSelected);
	}
}

bool	CXYPlot::WriteToFile( tstring pathName )
{
	tofstream			ofs;
	int					whichDataSet;
	CPlotData<DataPoint2D>		*dataSet;
	
	if( GetDataSetCount() <= 0 ) return false;
	
#if defined(_UNICODE) || defined(UNICODE)
	ofs.open(mywcs2mbs(pathName).c_str(), tios::app);
#else
	ofs.open(pathName.c_str(),tios::app);
#endif
	
	if(!ofs.is_open())return false;
	
	for( whichDataSet = 0; whichDataSet < GetDataSetCount(); whichDataSet++ )
	{
		dataSet = GetAbsoluteDataSet(whichDataSet);
		
		if( dataSet == NULL ) continue;
		if(!dataSet->WriteToFile(ofs))
		{
			ofs.close();
			return false;
		}
		if( whichDataSet != GetDataSetCount() - 1 )
		{
			ofs.write(_TEXT("\r\n"),_tcslen(_TEXT("\r\n")));
			ofs.write(CGlobal::m_strFileSeparator.c_str(), CGlobal::m_strFileSeparator.length());
			ofs.write(_TEXT("\r\n"),_tcslen(_TEXT("\r\n")));
		}
	}
	ofs.close();
	
	return true;
}

bool	CXYPlot::WriteToFile( int which, tstring pathName )
{
	if( which < 0 || which >= GetDataSetCount() ) return false;
	
	CPlotData<DataPoint2D>		*dataSet = GetAbsoluteDataSet(which);
	
	if( dataSet == NULL ) return false;
	
	if(!dataSet->WriteToFile(pathName))return false;
	
	return true;
}

bool	CXYPlot::ReadFromFile( tstring pathName )
{
	tifstream			ifs;
#if defined(_UNICODE) || defined(UNICODE)
	ifs.open(mywcs2mbs(pathName).c_str());
#else
	ifs.open(pathName.c_str());
#endif
	if(!ifs.is_open())return false;

	int filesize=get_file_size(pathName.c_str());
	TCHAR *pBuff=new TCHAR[filesize+1];
	memset(pBuff,0,(filesize+1)*sizeof(TCHAR));
	ifs.read(pBuff,filesize);
	tstring content=pBuff;
	delete []pBuff;
	ifs.close();

	tstring strSep = CGlobal::m_strFileSeparator;
	tstring::size_type pos;

	trimspace(strSep);

	tstring buffUsed, buffLeft;
	
	pos = content.find_first_of(strSep);
	if(pos == tstring::npos)
	{
		buffUsed = content;
		buffLeft = _TEXT("");
	}
	else
	{
		buffUsed = content.substr(0, pos);
		buffLeft = content.substr(pos + CGlobal::m_strFileSeparator.length());
	}

	CPlotData<DataPoint2D> dataSet;
	int count = 0;
	while(buffUsed != _TEXT(""))
	{
		if( !dataSet.ReadFromBuff( buffUsed ) )continue;
		int dataID = AddData(dataSet.GetData());
		SetDataTitle( dataID, dataSet.GetTitle() );
		count ++;
		
		pos = buffLeft.find_first_of(strSep);
		if(pos == tstring::npos)
		{
			buffUsed = buffLeft;
			buffLeft = _TEXT("");
		}
		else
		{
			buffUsed = buffLeft.substr(0, pos);
			buffLeft = buffLeft.substr(pos + strSep.length());
		}
	}
	return count>0;
}

bool	CXYPlot::ReadFromFile( int which, tstring pathName )
{
	if( which < 0 || which >= GetDataSetCount() ) return false;
	
	CPlotData<DataPoint2D>	*dataSet = GetAbsoluteDataSet(which);
	if( dataSet == NULL ) return false;
	
	if( !dataSet->ReadFromFile( pathName ) )return false;
	SetDataModified( GetDataID( which ) );
	
	return true;
}