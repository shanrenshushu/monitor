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

#ifndef __PLOT_H_122333444455555__
#define __PLOT_H_122333444455555__

#define WIN32_LEAN_AND_MEAN

#pragma warning(disable:4786)

#define DEFAULT_WHITE_BACKGROUND 1

#include "Global.h"
#include "ColorTable.h"
#include "MyConst.h"

#include <vector>
#include <math.h>

class CLegend;
class CAxis;

const int kMaxAxes = 8;

enum
{
	kPopupError = -1,
	kPopupNoAction = -2,
	kPopupUpdate = -3
};

enum
{
	kBrushNull = 0,
	kBrushSolid = 1,
	kBrushHatch = 2,
	kBrushPattern = 3,
		
	kBrushTypeCount
};

class CPlot// : public CGlobal
{
public:
	CPlot();
	virtual ~CPlot();

	void		SetDefaults();
	void		CopySettings(const CPlot *plot);

	void		DeleteAll();

private:
	// Variables
	RECT		m_rctLastClientRect;

protected:
	bool		m_bUseMajorHGrids;
	bool		m_bUseMajorVGrids;
	bool		m_bUseMinorHGrids;
	bool		m_bUseMinorVGrids;
	COLORREF	m_crMajorGridColor, m_crMinorGridColor;
	int			m_nMajorGridLineSize, m_nMinorGridLineSize;
	int			m_nMajorGridLineStyle, m_nMinorGridLineStyle;

	CAxis		*m_pAxes[kMaxAxes];
	int			m_nAxisCount;
	
	tstring		m_strTitle;
	LOGFONT		m_lfTitleFont;
	COLORREF	m_crTitleColor;
	bool		m_bTitleShow;
	
	COLORREF	m_crBorderColor;
	int			m_nBorderSize, m_nBorderStyle;
	bool		m_bBorderShow;

	COLORREF	m_crEdgeColor;
	int			m_nEdgeSize, m_nEdgeStyle;
	bool		m_bEdgeShow;
	bool		m_bEdgeButton;
	int			m_nEdgeType,m_nEdgeFlag;

	bool		m_bEraseBkgnd;
	int			m_nBkgndBrushType;
	COLORREF	m_crBkgndColor;
	int			m_nBkgndHatchStyle;
	HBITMAP		m_hBkgndPattern;
	bool		m_bGradientBkgnd;
	bool		m_bGradientHorizontal;
	COLORREF	m_crGradientColor1,m_crGradientColor2;

	bool		m_bUseDataRegionBrush;
	int			m_nDataRegionBrushType;
	COLORREF	m_crDataRegionColor;
	int			m_nDataRegionHatchStyle;
	HBITMAP		m_hDataRegionPattern;

	int			m_nMarginLeft;
	int			m_nMarginRight;
	int			m_nMarginTop;
	int			m_nMarginBottom;
	int			m_nMinMargin;
	
	bool		m_bEqualXYRange,m_bEqualXYAxis;
	
	tstring		m_strSubtitle;
	LOGFONT		m_lfSubtitleFont;
	COLORREF	m_crSubtitleColor;
	bool		m_bSubtitleShow;
	SIZE		m_sizeSubtitleOffset;

	bool		m_bSelectTitle;
	COLORREF	m_crSelectColor;
	bool		m_bLightTitle;
	bool		m_bLightDataRegion;
	COLORREF	m_crLightColor;
	int			m_nLightLineSize;
	int			m_nLightLineStyle;

	bool		m_bSwitchReverse;

	CLegend		*m_pLegend;
	bool		m_bUseLegend;
	bool		m_bPlotSelected;
	bool		m_bPlotLighted;

public:
	inline	bool		IsUseMajorHGrids(){return m_bUseMajorHGrids;}
	inline	void		SetUseMajorHGrids(bool bUse){m_bUseMajorHGrids=bUse;}
	inline	bool		IsUseMajorVGrids(){return m_bUseMajorVGrids;}
	inline	void		SetUseMajorVGrids(bool bUse){m_bUseMajorVGrids=bUse;}
	inline	bool		IsUseMinorHGrids(){return m_bUseMinorHGrids;}
	inline	void		SetUseMinorHGrids(bool bUse){m_bUseMinorHGrids=bUse;}
	inline	bool		IsUseMinorVGrids(){return m_bUseMinorVGrids;}
	inline	void		SetUseMinorVGrids(bool bUse){m_bUseMinorVGrids=bUse;}

	inline bool			IsTitleRealyShow( void ) { return m_bTitleShow && m_strTitle != _TEXT(""); }

	inline tstring&		GetTitle( void ) { return m_strTitle; }
	inline void			SetTitle( tstring title ) { m_strTitle = title; }
	inline bool			IsTitleShow( void ) { return m_bTitleShow; }
	inline void			SetTitleShow( bool show ) { m_bTitleShow = show; }
	inline COLORREF		GetTitleColor( void ) { return m_crTitleColor; }
	inline void			SetTitleColor( COLORREF color ) { m_crTitleColor = color; }
	inline LOGFONT&		GetTitleFont() { return m_lfTitleFont; }
	inline void			SetTitleFont( LOGFONT font ) { m_lfTitleFont = font; }

	inline tstring&		GetSubtitle( void ) { return m_strSubtitle; }
	inline void			SetSubtitle( tstring title ) { m_strSubtitle = title; }
	inline bool			IsSubtitleShow( void ) { return m_bSubtitleShow; }
	inline void			SetSubtitleShow( bool show ) { m_bSubtitleShow = show; }
	inline SIZE			GetSubtitleOffset( void ) { return m_sizeSubtitleOffset; }
	inline void			SetSubtitleOffset( SIZE offset ) { m_sizeSubtitleOffset=offset; }
	inline COLORREF		GetSubtitleColor( void ) { return m_crSubtitleColor; }
	inline void			SetSubtitleColor( COLORREF color ) { m_crSubtitleColor = color; }
	inline LOGFONT		&GetSubtitleFont() { return m_lfSubtitleFont; }
	inline void			SetSubtitleFont( LOGFONT font ) { m_lfSubtitleFont = font; }

	inline int			GetMajorGridLineSize( void ) { return m_nMajorGridLineSize; }
	inline void			SetMajorGridLineSize( int lineSize ) { if( lineSize > 0 ) m_nMajorGridLineSize = lineSize; }
	inline int			GetMinorGridLineSize( void ) { return m_nMinorGridLineSize; }
	inline void			SetMinorGridLineSize( int lineSize ) { if( lineSize > 0 ) m_nMinorGridLineSize = lineSize; }
	inline int			GetMajorGridLineStyle( void ) { return m_nMajorGridLineStyle; }
	inline void			SetMajorGridLineStyle( int lineStyle ) { m_nMajorGridLineStyle = lineStyle; }
	inline int			GetMinorGridLineStyle( void ) { return m_nMinorGridLineStyle; }
	inline void			SetMinorGridLineStyle( int lineStyle ) { m_nMinorGridLineStyle = lineStyle; }
	inline COLORREF		GetMajorGridColor( void ) { return m_crMajorGridColor; }
	inline void			SetMajorGridColor( COLORREF color ) { m_crMajorGridColor = color; }
	inline COLORREF		GetMinorGridColor( void ) { return m_crMinorGridColor; }
	inline void			SetMinorGridColor( COLORREF color ) { m_crMinorGridColor = color; }

	inline int			GetBkgndBrushType( void ){ return m_nBkgndBrushType; }
	inline void			SetBkgndBrushType( int type ){ m_nBkgndBrushType = type; }
	inline int			GetBkgndHatchStyle( void ){ return m_nBkgndHatchStyle; }
	inline void			SetBkgndHatchStyle( int style ){ m_nBkgndHatchStyle = style; }
	inline HBITMAP		GetBkgndPattern( void ){ return m_hBkgndPattern; }
	inline void			SetBkgndPattern( HBITMAP pattern ){ m_hBkgndPattern = pattern; }
	inline	COLORREF	GetBkgndColor( void ) { return m_crBkgndColor; }
	inline void			SetBkgndColor( COLORREF color ) { m_crBkgndColor = color; }
	inline	COLORREF	GetBorderColor( void ) { return m_crBorderColor; }
	inline void			SetBorderColor( COLORREF color ) { m_crBorderColor = color; }
	inline	int			GetBorderSize( void ) { return m_nBorderSize; }
	inline void			SetBorderStyle( int style ) { m_nBorderStyle = style; }
	inline	int			GetBorderStyle( void ) { return m_nBorderStyle; }
	inline void			SetBorderSize( int size ) { m_nBorderSize = size; }
	inline	bool		IsBorderShow( void ) { return m_bBorderShow; }
	inline	void		SetBorderShow( bool show ) { m_bBorderShow = show; }
	inline	bool		IsEraseBkgnd( void ) { return m_bEraseBkgnd; }
	inline void			SetEraseBkgnd( bool how ) { m_bEraseBkgnd = how; }

	inline	COLORREF	GetEdgeColor( void ) { return m_crEdgeColor; }
	inline void			SetEdgeColor( COLORREF color ) { m_crEdgeColor = color; }
	inline	int			GetEdgeSize( void ) { return m_nEdgeSize; }
	inline void			SetEdgeStyle( int style ) { m_nEdgeStyle = style; }
	inline	int			GetEdgeStyle( void ) { return m_nEdgeStyle; }
	inline void			SetEdgeSize( int size ) { m_nEdgeSize = size; }
	inline	bool		IsEdgeShow( void ) { return m_bEdgeShow; }
	inline	void		SetEdgeShow( bool show ) { m_bEdgeShow = show; }
	inline	bool		IsEdgeButton( void ) { return m_bEdgeButton; }
	inline	void		SetEdgeButton( bool butt ) { m_bEdgeButton = butt; }
	inline	int			GetEdgeType( void ) { return m_nEdgeType; }
	inline	void		SetEdgeType( int type ) { m_nEdgeType = type; }
	inline	int			GetEdgeFlag( void ) { return m_nEdgeFlag; }
	inline	void		SetEdgeFlag( int flag ) { m_nEdgeFlag = flag; }

	inline int			GetMarginLeft(){ return m_nMarginLeft; }
	inline void			SetMarginLeft(int margin){ m_nMarginLeft = margin; }
	inline int			GetMarginRight(){ return m_nMarginRight; }
	inline void			SetMarginRight(int margin){ m_nMarginRight = margin; }
	inline int			GetMarginTop(){ return m_nMarginTop; }
	inline void			SetMarginTop(int margin){ m_nMarginTop = margin; }
	inline int			GetMarginBottom(){ return m_nMarginBottom; }
	inline void			SetMarginBottom(int margin){ m_nMarginBottom = margin; }

	inline bool			IsEqualXYRange(){ return m_bEqualXYRange; }
	inline void			SetEqualXYRange( bool equal ){ m_bEqualXYRange=equal; }
	inline bool			IsEqualXYAxis(){ return m_bEqualXYAxis; }
	inline void			SetEqualXYAxis( bool equal ){ m_bEqualXYAxis=equal; }

	inline int			GetDataRegionBrushType( void ){ return m_nDataRegionBrushType; }
	inline void			SetDataRegionBrushType( int type ){ m_nDataRegionBrushType = type; }
	inline int			GetDataRegionHatchStyle( void ){ return m_nDataRegionHatchStyle; }
	inline void			SetDataRegionHatchStyle( int style ){ m_nDataRegionHatchStyle = style; }
	inline HBITMAP		GetDataRegionPattern( void ){ return m_hDataRegionPattern; }
	inline void			SetDataRegionPattern( HBITMAP pattern ){ m_hDataRegionPattern = pattern; }
	inline COLORREF		GetDataRegionColor( void ) { return m_crDataRegionColor; }
	inline void			SetDataRegionColor( COLORREF color ) { m_crDataRegionColor = color; }
	inline bool			IsUseDataRegionBrush( void ) { return m_bUseDataRegionBrush; }
	inline void			SetUseDataRegionBrush( bool use ) { m_bUseDataRegionBrush = use; }
	inline bool			IsGradientBkgnd( void ) { return m_bGradientBkgnd; }
	inline void			SetGradientBkgnd( bool set) { m_bGradientBkgnd=set; }
	inline bool			IsGradientHorizontal( void ) { return m_bGradientHorizontal; }
	inline void			SetGradientHorizontal( bool hor ){ m_bGradientHorizontal=hor; }
	inline COLORREF		GetGradientColor1( void ) { return m_crGradientColor1; }
	inline void			SetGradientColor1( COLORREF color ) { m_crGradientColor1=color; }
	inline COLORREF		GetGradientColor2( void ) { return m_crGradientColor2; }
	inline void			SetGradientColor2( COLORREF color ) { m_crGradientColor2=color; }

	inline bool			IsSelectTitle() { return m_bSelectTitle; }
	inline void			SetSelectTitle( bool select ) { m_bSelectTitle = select; }
	inline COLORREF		GetSelectColor() { return m_crSelectColor; }
	inline void			SetSelectColor( COLORREF color ) { m_crSelectColor = color; }
	inline bool			IsLightTitle() { return m_bLightTitle; }
	inline void			SetLightTitle( bool light ) { m_bLightTitle=light; }
	inline bool			IsLightDataRegion() { return m_bLightDataRegion; }
	inline void			SetLightDataRegion( bool light ) { m_bLightDataRegion=light; }
	inline COLORREF		GetLightColor( void ) { return m_crLightColor; }
	inline void			SetLightColor( COLORREF color ) { m_crLightColor = color; }
	inline int			GetLightLineSize( void ) { return m_nLightLineSize; }
	inline void			SetLightLineSize( int size ) { m_nLightLineSize = size; }
	inline int			GetLigntLineStyle(){return m_nLightLineStyle;}
	inline void			SetLightLineStyle(int style){m_nLightLineStyle = style;}

	inline bool			IsSwitchReverse(){return m_bSwitchReverse;}
	inline void			SetSwitchReverse(bool reverse){m_bSwitchReverse = reverse;}

	inline bool			IsPlotSelected(){return m_bPlotSelected;}
	inline void			SetPlotSelected( bool bSel){m_bPlotSelected = bSel;}
	inline bool			IsPlotLighted(){return m_bPlotLighted;}
	inline void			SetPlotLighted( bool bLight){m_bPlotLighted = bLight;}
	
	inline bool			IsUseLegend() { return m_bUseLegend; }
	inline void			SetUseLegend(bool use) { m_bUseLegend = use; }
	CLegend				*GetLegend();
	virtual CLegend		*NewLegend()=0;
	void				DeleteLegend();

public:
	inline RECT			GetLastClientRect(){return m_rctLastClientRect;}
	inline void			SetLastClientRect(RECT rt){m_rctLastClientRect=rt;}
	
	int					GetAxisCount( ){ return m_nAxisCount; }
	inline	CAxis		*GetAxis( int which ) { if( which >= 0 && which < m_nAxisCount ) return m_pAxes[which]; else return NULL; }
	virtual	CAxis		*AddAxis( int location );
	virtual CAxis		*NewAxis()=0;
	CAxis				*GetAxisByLocation( int location );
	int					GetAxisIndex( int location );
	bool				IsAxesVisible();
	int					GetVisibleAxisCount();
	void				RejectAxisByLocation(int location);
	void				RejectAxis( int which );// do not delete really, only not use it
	void				RejectAllAxes();
	void				DeleteAllAxes();
	int					GetXAxisCount();
	int					GetVisibleXAxisCount();
	int					GetYAxisCount();
	int					GetVisibleYAxisCount();
	int					GetIndexOfFirstVisibleAxis();
	int					GetIndexOfLastVisibleAxis();
	int					GetIndexOfNextVisibleAxis(int idxCurr);
	int					GetIndexOfPrevVisibleAxis(int idxCurr);

	virtual	bool		IsAxisLighted();
	virtual	void		UnlightAllAxis();
	virtual	bool		IsAxisSelected();
	virtual	void		UnselectAllAxis();
	int					GetIndexOfLightedAxis();
	int					GetIndexOfSelectedAxis();

	void				DeleteUsedHandles();

	virtual void		GetPlotRange( double *xRange, double *yRange )=0;
	void				GetRange( double *xRange, double *yRange );
	void				GetXRange( double *range );
	void				GetYRange( double *range );
	void				SetRange( bool setX, double low, double high );
	void				SetXRange( double low, double high );
	void				SetYRange( double low, double high );
	void				IsAutoRange( bool &haveXRange, bool &haveYRange );
	bool				IsXAutoRange( void );
	bool				IsYAutoRange( void );
	void				SetAutoRange( bool setX );
	void				SetXAutoRange( void );
	void				SetYAutoRange( void );

	virtual tstring		GetAbsoluteDataTitle( int which ){return _TEXT("");};

public:
	virtual	bool		IsEmpty()=0;

public:
	static	int			ChooseLineWidth(int linewidth, bool bLighted, bool bSelected);
	static	COLORREF	ChooseLineColor(COLORREF linecolor, bool bLighted, bool bSelected);
};

#endif