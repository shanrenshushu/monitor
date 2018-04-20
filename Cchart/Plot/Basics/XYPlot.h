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

#ifndef __XYPLOT_H_122333444455555__
#define __XYPLOT_H_122333444455555__

#include "Plot.h"
#include "PlotData.h"

// Plot type
enum
{
	kXYPlotScatter = 0,
	kXYPlotConnect = 1,
	kXYPlotStepHV = 2,
	kXYPlotStepVH = 3,
	kXYPlotStepHVH = 4,
	kXYPlotStepVHV = 5,
	kXYPlotBezier = 6,
	
	kPlotTypeCount
};

// Marker type
enum
{
	kXYMarkerNone = 0,
	kXYMarkerCircle = 1,
	kXYMarkerSquareUpright = 2,
	kXYMarkerSquareOblique = 3,
	kXYMarkerTriangleLeft = 4,
	kXYMarkerTriangleRight = 5,
	kXYMarkerTriangleUp = 6,
	kXYMarkerTriangleDown = 7,
	kXYMarkerX = 8,
	kXYMarkerDot = 9,
	
	kMarkerTypeCount
};

// Method of set colors
enum
{
	kSetColorFromTable = 0,
	kSetColorFromTwoColor = 1,
	
	kSetColorModeCount
};

// how to show dataset infomation
enum
{
	kShowDataSetInfoNot = 0,
	kShowDataSetInfoEveryN = 1,
	kShowDataSetInfoTotalN = 2,
	
	kShowDataSetInfoModeCount
};

// how to fill the curve
enum
{
	kDataFillClosed = 0,
	kDataFillFromBottomAxis = 1,
	kDataFillFromTopAxis = 2,
	kDataFillFromLeftAxis = 3,
	kDataFillFromRightAxis = 4,

	kDataFillModeCount
};

typedef struct stXYDataInfo
{
	CPlotData<DataPoint2D>	*pDataSet;
	int			nDataID;

	int			nPlotType; // see above enum

	COLORREF	crDataColor; // An RGB value for initial color
	COLORREF	crPlotColor; // An RGB value for really plotting
	bool		bAutoColor; // Let the program choose color

	int			nMarkerType;	// how to mark data values (see enum)
	int			nMarkerSize;
	int			nMarkerFrequency;
	bool		bMarkerFill;

	double		fXDataMin;
	double		fXDataMax;
	double		fYDataMin;
	double		fYDataMax;
	bool		bDataRangesSet;

	int			nDataLineSize;
	int			nPlotLineSize;

	int			nDataLineStyle;
	int			nPlotLineStyle;
	bool		bAutoLineStyle;

	int			nDataFillMode;
	bool		bDataFill;
	int			nDataFillBrushType;
	COLORREF	crDataFillColor;
	int			nDataFillHatchStyle;
	HBITMAP		hDataFillPattern;

	bool		bVisible;
	bool		bSelected;
	bool		bLighted;
	bool		bReact;	
}XYDataInfo, *pXYDataInfo;
typedef vector<pXYDataInfo> vpXYDataInfo;

// class CPlot is a basement class
class CXYPlot
{
public:
	CXYPlot();
	virtual ~CXYPlot();

	virtual	void		SetDefaults( int index );
	void				SetDefaults( );
	void				CopySettings( const CXYPlot *plot );
	void				CopySettings( const CXYPlot *plot , int index);
	void				CopyAll( const CXYPlot *plot );

	bool				DeleteData( int dataID );
	void				DeleteAllData( void );
	void				DeleteAll( void );

	bool				SetDataModified( int dataID );
protected:
	vpXYDataInfo		m_vpXYDataInfo;
	int					m_nCurDataID;

	bool				m_bMultiSelect;
	
	int					m_nShowDataSetInfoMode;
	int					m_nShowDataSetInfoNum;
	int					m_nShowDataSetInfoDataPointStep;
	
	bool				m_bKeepFormer;

	bool				m_bAutoDataColor;
	int					m_nModeOfSetColors;
	COLORREF			m_crLineColor1,m_crLineColor2;
	
	bool				m_bAutoLineStyle;

	double				m_fHitPrecision;

public:
	int					AddCurve( double *pX, double *pY, int nLen );
	int					AddCurve( MyVData2D vData,int plottype = kXYPlotConnect,COLORREF color = RGB(0,0,0),int markertype = kXYMarkerCircle,int markerfreq = 0){ return AddDataSet(vData, plottype, color, markertype, markerfreq); }
	int					AddDataSet( MyVData2D vData,int plottype, COLORREF color, int markertype, int markerfreq );

public:
	vpXYDataInfo		GetXYDataInfo() { return m_vpXYDataInfo; }
	inline int			GetXYDataInfoSize() {return m_vpXYDataInfo.size(); }

	inline int			GetCurDataID() { return m_nCurDataID; }

	inline bool			IsMultiSelect(){return m_bMultiSelect;}
	inline void			SetMultiSelect(bool bSel){m_bMultiSelect=bSel;}
	inline void			SetShowDataSetInfoMode(int mode){m_nShowDataSetInfoMode=mode;}
	inline int			GetShowDataSetInfoMode(){return m_nShowDataSetInfoMode;}
	inline void			SetShowDataSetInfoNum(int num){m_nShowDataSetInfoNum=num;}
	inline int			GetShowDataSetInfoNum(){return m_nShowDataSetInfoNum;}
	inline void			SetShowDataSetInfoDataPointStep(int step){m_nShowDataSetInfoDataPointStep=step;}
	inline int			GetShowDataSetInfoDataPointStep(){return m_nShowDataSetInfoDataPointStep;}
	inline bool			IsKeepFormer() { return m_bKeepFormer;}
	inline void			SetKeepFormer( bool keep ) { m_bKeepFormer=keep; }

	inline bool			IsAutoLineStyle(){return m_bAutoLineStyle;}
	inline void			SetAutoLineStyle(bool bAuto){m_bAutoLineStyle=bAuto;}
	inline bool			IsAutoDataColor(){return m_bAutoDataColor;}
	inline void			SetAutoDataColor(bool bAuto){m_bAutoDataColor=bAuto;}
	inline int			GetModeOfSetColors(){return m_nModeOfSetColors;}
	inline void			SetModeOfSetColors(int mode){m_nModeOfSetColors=mode;};
	inline COLORREF		GetLineColor1(){return m_crLineColor1;}
	inline void			SetLineColor1(COLORREF color){m_crLineColor1=color;}
	inline COLORREF		GetLineColor2(){return m_crLineColor2;}
	inline void			SetLineColor2(COLORREF color){m_crLineColor2=color;}
	inline double		GetHitPrecision(){return m_fHitPrecision;}
	inline void			SetHitPrecision( double val ){m_fHitPrecision=val;}

public:
	inline void			SetDataColor( int dataID, COLORREF color ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpXYDataInfo[i]->crDataColor=color;}
	inline COLORREF		GetDataColor( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return RGB( 0, 0, 0); else return m_vpXYDataInfo[i]->crDataColor; }
	inline void			SetPlotColor( int dataID, COLORREF color ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpXYDataInfo[i]->crPlotColor=color;}
	inline COLORREF		GetPlotColor( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return RGB( 0, 0, 0); else return m_vpXYDataInfo[i]->crPlotColor; }
	inline void			SetAutoColor( int dataID, bool bAuto){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpXYDataInfo[i]->bAutoColor=bAuto;}
	inline bool			IsAutoColor( int dataID ) {int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpXYDataInfo[i]->bAutoColor;}
	inline void			SetMarkerType( int dataID, int type ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpXYDataInfo[index]->nMarkerType = type;}
	inline int			GetMarkerType( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpXYDataInfo[i]->nMarkerType; }
	inline void			SetMarkerSize( int dataID, int size ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpXYDataInfo[index]->nMarkerSize = size;}
	inline int			GetMarkerSize( int dataID )  { int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpXYDataInfo[i]->nMarkerSize; }
	inline void			SetMarkerFillState( int dataID, bool how ){	int	index = GetIndex( dataID );	if( index < 0 ) return;	m_vpXYDataInfo[index]->bMarkerFill = how;}
	inline bool			GetMarkerFillState( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpXYDataInfo[i]->bMarkerFill; }
   
	inline void			SetPlotType( int dataID, int type ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpXYDataInfo[index]->nPlotType = type;}
	inline int			GetPlotType( int dataID )   { int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpXYDataInfo[i]->nPlotType; }
	inline void			SetMarkerFrequency( int dataID, int freq ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpXYDataInfo[index]->nMarkerFrequency = freq;}
	inline int			GetMarkerFrequency( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->nMarkerFrequency; }
	inline int			GetDataLineSize( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpXYDataInfo[i]->nDataLineSize; }
	inline void			SetDataLineSize( int dataID, int lineSize ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nDataLineSize = lineSize; }
	inline int			GetPlotLineSize( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpXYDataInfo[i]->nPlotLineSize; }
	inline void			SetPlotLineSize( int dataID, int lineSize ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nPlotLineSize = lineSize; }
	inline int			GetDataLineStyle( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpXYDataInfo[i]->nDataLineStyle; }
	inline void			SetDataLineStyle( int dataID, int lineStyle ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nDataLineStyle = lineStyle; }
	inline int			GetPlotLineStyle( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpXYDataInfo[i]->nPlotLineStyle; }
	inline void			SetPlotLineStyle( int dataID, int lineStyle ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nPlotLineStyle = lineStyle; }
	inline bool			IsAutoLineStyle( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return false; else return m_vpXYDataInfo[i]->bAutoLineStyle; }
	inline void			SetAutoLineStyle( int dataID, bool bAuto ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->bAutoLineStyle = bAuto; };
	inline bool			IsVisible( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->bVisible; }
	inline void			SetVisible( int dataID, bool bShow ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->bVisible = bShow; };
	inline bool			IsSelected( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->bSelected; }
	inline void			SetSelected( int dataID, bool bSel ) { int i = GetIndex( dataID ); if( i < 0 ) return; else {if(!m_bMultiSelect && bSel)SelectAllDataSets(false);m_vpXYDataInfo[i]->bSelected = bSel;} };
	inline bool			IsLighted( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->bLighted; }
	inline void			SetLighted( int dataID, bool bLight ){UnlightAllDataSets();int i = GetIndex( dataID );if( i < 0 ) return;else m_vpXYDataInfo[i]->bLighted = bLight;}

	inline bool			IsReact( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpXYDataInfo[i]->bReact; }
	inline void			SetReact( int dataID, bool bFlag ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->bReact = bFlag; }
	
	inline int			GetDataFillMode( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->nDataFillMode; }
	inline void			SetDataFillMode( int dataID, int mode ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nDataFillMode = mode; }
	inline bool			IsDataFill( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->bDataFill; }
	inline void			SetDataFill( int dataID, bool bFill ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->bDataFill = bFill; }
	inline int			GetDataFillBrushType( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->nDataFillBrushType; }
	inline void			SetDataFillBrushType( int dataID, int type ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nDataFillBrushType = type; }
	inline COLORREF		GetDataFillColor( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpXYDataInfo[i]->crDataFillColor; }
	inline void			SetDataFillColor( int dataID, COLORREF color ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->crDataFillColor = color; }
	inline int			GetDataFillHatchStyle( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpXYDataInfo[i]->nDataFillHatchStyle; }
	inline void			SetDataFillHatchStyle( int dataID, int style ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->nDataFillHatchStyle = style; }
	inline HBITMAP		GetDataFillPattern( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return NULL; else return m_vpXYDataInfo[i]->hDataFillPattern; }
	inline void			SetDataFillPattern( int dataID, HBITMAP pattern ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpXYDataInfo[i]->hDataFillPattern = pattern; }

	inline	bool		DelDataSetSel(){for(int i=0;i<GetDataSetCount();i++){if(m_vpXYDataInfo[i]->bSelected)if(!DeleteData(GetDataID(i)))return false;}return true;}
	inline	bool		HideDataSetSel(){for(int i=0;i<GetDataSetCount();i++)if(IsSelected(GetDataID(i)))SetVisible(GetDataID(i),false);return true;}
	inline	virtual	void		SelectAllDataSets(bool bSel){for(int i=0;i<GetDataSetCount();i++)m_vpXYDataInfo[i]->bSelected=bSel;}
	inline	virtual bool		IsDataSetSelected(){for(int i=0;i<GetDataSetCount();i++)if(m_vpXYDataInfo[i]->bSelected)return true;return false;}
	inline	virtual	bool		IsDataSetLighted(){for(int i=0;i<GetDataSetCount();i++)if(m_vpXYDataInfo[i]->bLighted)return true;return false;}
	inline	virtual	void		UnlightAllDataSets(){for(int i=0;i<GetDataSetCount();i++)m_vpXYDataInfo[i]->bLighted=false;}
	inline	int			GetIdOfLighted(){for(int i=0;i<GetDataSetCount();i++)if(m_vpXYDataInfo[i]->bLighted)return GetDataID(i);return -1;}
	inline	int			GetIndexOfLighted(){for(int i=0;i<GetDataSetCount();i++)if(m_vpXYDataInfo[i]->bLighted)return i;return -1;}
	inline	bool		IsAnyDataSetHiden(){for(int i=0;i<GetDataSetCount();i++)if(!m_vpXYDataInfo[i]->bVisible)return true;return false;}
	inline	void		ShowAllDataSets(){for(int i=0;i<GetDataSetCount();i++)SetVisible(GetDataID(i),true);}
	inline	int			GetIdOfSelected(){for(int i=0;i<GetDataSetCount();i++)if(m_vpXYDataInfo[i]->bSelected)return GetDataID(i);return -1;}
	inline	int			GetIndexOfSelected(){for(int i=0;i<GetDataSetCount();i++)if(m_vpXYDataInfo[i]->bSelected)return i;return -1;}
	
	inline	virtual int			GetDataSetCount( void ) const { return m_vpXYDataInfo.size(); }// virtual for LayeredLegend
	inline	virtual int			GetDataSetLevels() const {return GetDataSetCount();}

	int					GetIndexOfFirstVisibleDataSet();
	int					GetIndexOfLastVisibleDataSet();
	int					GetIndexOfNextVisibleDataSet(int idxCurr);
	int					GetIndexOfPrevVisibleDataSet(int idxCurr);

	virtual	int					GetVisibleDataSetCount( );

public:
	tstring				GetDataTitle( int dataID );
	void				SetDataTitle( int dataID, tstring title );

	int					GetDataID( int index) const
	{
		if(index<0 || index>=GetDataSetCount())return -1;
		else return m_vpXYDataInfo[index]->nDataID;
	};
	int					GetIndex( int dataID ) const;

	virtual CPlotData<DataPoint2D>	*GetAbsoluteDataSet( int which );// virtual for LayeredLegend
	//virtual tstring					GetAbsoluteDataTitle( int which );
	CPlotData<DataPoint2D>			*GetDataSet( int dataID );

	const MyVData2D		&GetData( int dataID ) const;
	const MyVData2D		&GetAbsoluteData( int which ) const;

	void				GetDataRange( int dataID );
	void				GetDataRanges();
	void				GetDataRanges(double xRange[2], double yRange[2]);

public:
	bool				WriteToFile( tstring pathName );
	bool				WriteToFile( int which, tstring pathName );
	bool				ReadFromFile( tstring pathName );
	bool				ReadFromFile( int which, tstring pathName );
	//csh
	int					SetData( int dataID, MyVData2D vData );
protected:
	void				SetPlotColors();
	void				SetPlotLineStyles();
	void				SetPlotLineSizes();
	
private:
	virtual	int			AddData( MyVData2D vData );
	
};

#endif