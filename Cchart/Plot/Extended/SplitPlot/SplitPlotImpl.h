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

#ifndef __SPLITPLOTIMPL_H_122333444455555__
#define __SPLITPLOTIMPL_H_122333444455555__

#include "../../Basics/XYHandler.h"
#include "../../Basics/XYPlotImpl.h"
#include "SplitPlot.h"

// This templated class
template <typename CXYPlotImplT>
class CSplitPlotImpl : public CSplitPlot<CXYPlotImplT>, public CPlotImpl
{
public:
	CSplitPlotImpl();
	virtual ~CSplitPlotImpl();

	void	OnDraw( HWND hWnd );
	void	OnDraw( HDC hDC, RECT destRect );

	int		GetPlotIndex( POINT point );

	RECT	GetPlotRect(RECT clientRect,int nIndex);

	RECT	GetPlotRect( HDC hDC, RECT clientRect );
	RECT	GetTitleRect( HDC hDC, RECT clientRect, RECT chartRect );
	RECT	GetTitleRect( HDC hDC );

	// CPlotImpl is a pure virtual class, so the following functions should be defined
	virtual CAxis	*NewAxis(){ return 0; }
	virtual CLegend	*NewLegend(){ return 0; }
	virtual	bool	IsEmpty(){ return GetDataSetCount()<=0; }
	virtual void	GetPlotRange( double *xRange, double *yRange ){}

	//add the following function since m_rctLastClientRect have inherited twice
	RECT			GetLastClientRect(){return CSplitPlot<CXYPlotImplT>::GetLastClientRect();}
	void			SetLastClientRect(RECT rt){CSplitPlot<CXYPlotImplT>::SetLastClientRect(rt);}

public:
	void	SwitchSelection();
	void	UnlightAll();
	void	UnselectAll();

public:
	POINT	m_ptOrigin,m_ptCurr;
	int		m_nPositionOrigin;
	int		m_nSpliterIndex;
	bool	m_bDragRowSpliter, m_bDragColSpliter;
};

template <typename SplitPlotImplT>
class CSplitHandler : public CHandler
{
public:
	bool	OnLButtonDown( HWND hWnd, POINT point );
	bool	OnLButtonUp( HWND hWnd, POINT point );
	bool	OnLButtonDblClk( HWND hWnd, POINT point );
	bool	OnMouseMove( HWND hWnd, POINT point );
	bool	OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	bool	OnKeyDown( HWND hWnd, UINT key );
	
	//void	Interactive(HWND hWnd, UINT message, POINT point);
	void	Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

template <typename XYPlotImplT>
class CSplitPlotImplI : public CSplitPlotImpl<XYPlotImplT>, public CSplitHandler<CSplitPlotImplI<XYPlotImplT> >
{

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Implimentations of CSplitPlotImpl

template <typename CXYPlotImplT>
CSplitPlotImpl<CXYPlotImplT>::CSplitPlotImpl()
{
	m_bDragRowSpliter = false;
	m_bDragColSpliter = false;
}

template <typename CXYPlotImplT>
CSplitPlotImpl<CXYPlotImplT>::~CSplitPlotImpl()
{
}

template <typename CXYPlotImplT>
RECT	CSplitPlotImpl<CXYPlotImplT>::GetPlotRect(RECT clientRect,int nIndex)
{
	int iRow,iCol;
	RECT rect={0,0,0,0};

	switch( m_nSplitMode )
	{
	case kSplitNot:
		rect = clientRect;
		break;
	case kSplitNM:
		iRow = nIndex / m_nCols;
		iCol = nIndex % m_nCols;

		rect.top = m_vnRowSpliter[iRow];
		rect.bottom = m_vnRowSpliter[iRow+1];
		rect.left = m_vnColSpliter[iCol];
		rect.right = m_vnColSpliter[iCol+1];

		break;
	case kSplit3L2R1:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[1],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[1],m_vnColSpliter[1],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	case kSplit3L1R2:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[1],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[1],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	case kSplit3T2B1:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[1],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[1],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	case kSplit3T1B2:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[1],m_vnColSpliter[1],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[1],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	default:
		rect = clientRect;
		break;
	}
	return rect;
}

template <typename CXYPlotImplT>
RECT	CSplitPlotImpl<CXYPlotImplT>::GetPlotRect( HDC hDC, RECT clientRect )
{
	SIZE titleSize = GetTitleSize(hDC);
	RECT plotRect = clientRect;
	plotRect.top += titleSize.cy;

	return plotRect;
}

template <typename CXYPlotImplT>
RECT	CSplitPlotImpl<CXYPlotImplT>::GetTitleRect( HDC hDC, RECT clientRect, RECT chartRect )
{
	RECT titleRect = clientRect;
	titleRect.left = chartRect.left;
	titleRect.right = chartRect.right;
	titleRect.bottom = chartRect.top;
	
	return titleRect;
}

template <typename CXYPlotImplT>
RECT	CSplitPlotImpl<CXYPlotImplT>::GetTitleRect( HDC hDC )
{
	RECT titleRect = GetLastClientRect();
	SIZE titleSize = GetTitleSize(hDC);

	titleRect.bottom = titleRect.top + titleSize.cy;
	
	return titleRect;
}

template <typename CXYPlotImplT>
int		CSplitPlotImpl<CXYPlotImplT>::GetPlotIndex( POINT point )
{
	int i;

	int iCol=-1,iRow=-1;
	
	for( i=0; i<m_nCols; i++)
	{
		if( point.x >= m_vnColSpliter[i] && point.x < m_vnColSpliter[i+1] )
		{
			iCol = i;
			break;
		}
	}
	for( i=0; i<m_nRows; i++)
	{
		if( point.y >= m_vnRowSpliter[i] && point.y < m_vnRowSpliter[i+1] )
		{
			iRow = i;
			break;
		}
	}
	if(iCol<0 || iRow<0)
	{
		return -1;
	}

	int idx;
	switch(m_nSplitMode)
	{
	case kSplitNot:
		return 0;
	case kSplitNM:
		return iRow * m_nCols + iCol;
	case kSplit3L2R1://(iR,iC),(0,0)=0,(0,1)=2,(1,0)=1,(1,1)=2
		idx = iRow + 2*iCol;
		if(idx==3)idx=2;
		return idx;
	case kSplit3L1R2://(iR,iC),(0,0)=0,(0,1)=1,(1,0)=0,(1,1)=2
		idx = iRow + iCol;
		if(iRow ==1 && iCol ==0 )idx=0;
		return idx;
	case kSplit3T2B1://(iR,iC),(0,0)=0,(0,1)=1,(1,0)=2,(1,1)=2
		idx = 2*iRow + iCol;
		if(idx==3)idx=2;
		return idx;
	case kSplit3T1B2://(iR,iC),(0,0)=0,(0,1)=0,(1,0)=1,(1,1)=2
		idx = iRow + iCol;
		if(iRow ==0 && iCol ==1 )idx=0;
		return idx;
	default:
		return -1;
	}
}

template <typename CXYPlotImplT>
void	CSplitPlotImpl<CXYPlotImplT>::OnDraw( HWND hWnd )
{
	HDC hDC = GetDC(hWnd);
	RECT destRect;
	GetClientRect(hWnd, &destRect);

	OnDraw( hDC, destRect );

	ReleaseDC(hWnd, hDC);
}

template <typename CXYPlotImplT>
void	CSplitPlotImpl<CXYPlotImplT>::OnDraw(HDC hDC, RECT destRect)
{
	SetLastClientRect(destRect);
	
	EraseBkgnd(hDC, destRect);
	if(m_bTitleShow)
	{
		SIZE titleSize = GetTitleSize(hDC);
		RECT titleRect = destRect;
		titleRect.bottom = titleRect.top + titleSize.cy;
		
		DrawTitle( hDC, titleRect );
		
		destRect.top += titleSize.cy;
	}

	InitSpliters(destRect);
	UseColOffsets();
	UseRowOffsets();
	
	for(int i=0; i<GetPlotsCount(m_nSplitMode, m_nRows, m_nCols); i++)
	{
		m_vpPlots[i]->OnDraw(hDC,GetPlotRect(destRect,i) );
	}
	
}

template <typename CXYPlotImplT>
void	CSplitPlotImpl<CXYPlotImplT>::SwitchSelection()
{
	if(!IsTitleRealyShow() && GetPlotsCount()<=0) return;
	if(IsSelectTitle())
	{
		if(GetPlotsCount()<=0)return;
		SetSelectTitle(false);
		if(!m_bSwitchReverse)
			GetPlot(0)->SetPlotSelected(true);
		else
			GetPlot(GetPlotsCount()-1)->SetPlotSelected(true);
	}
	else if(IsSubplotSelected())
	{
		int i, index = GetIndexOfSelectedSubplot();
		if(!m_bSwitchReverse)
			i = index+1;
		else
			i = index-1;
		
		GetPlot(index)->SetPlotSelected(false);
		if(i>=0 && i<GetPlotsCount())
		{
			GetPlot(i)->SetPlotSelected(true);
		}
		else
		{
			if(IsTitleRealyShow())
				SetSelectTitle(true);
		}
	}
	else
	{
		if(IsTitleRealyShow())
			SetSelectTitle(true);
	}
}

template <typename CXYPlotImplT>
void	CSplitPlotImpl<CXYPlotImplT>::UnlightAll()
{
	if(IsTitleRealyShow())
		SetLightTitle(false);
	for(int i=0; i<GetPlotsCount(); i++)
	{
		GetPlot(i)->SetPlotLighted(false);
	}
}

template <typename CXYPlotImplT>
void	CSplitPlotImpl<CXYPlotImplT>::UnselectAll()
{
	if(IsTitleRealyShow())
		SetSelectTitle(false);
	for(int i=0; i<GetPlotsCount(); i++)
	{
		GetPlot(i)->SetPlotSelected(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//

template <typename SplitPlotImplT>
bool	CSplitHandler<SplitPlotImplT>::OnLButtonDown( HWND hWnd, POINT point )
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	HDC hDC = GetDC( hWnd );
	SetCapture(hWnd);

	int rowspliter, colspliter;
	bool bcol = pT->HitColSpliter(point, colspliter);
	bool brow = pT->HitRowSpliter(point, rowspliter);
	if( bcol || brow )
	{
		RECT clientRect, clipRect;
		GetClientRect(hWnd, &clientRect);
		clipRect = clientRect;
		if(bcol && colspliter > 0)
		{
			pT->m_bDragColSpliter = true;
			pT->m_bDragRowSpliter = false;
			pT->m_nSpliterIndex = colspliter;
			pT->m_nPositionOrigin = pT->m_vnColSpliter[colspliter];

			clipRect.left = pT->m_vnColSpliter[colspliter-1]+40;
			clipRect.right = pT->m_vnColSpliter[colspliter+1]-40;
			ClientToScreen(hWnd,(LPPOINT)&clipRect);
			ClientToScreen(hWnd,(LPPOINT)&clipRect+1);
			ClipCursor(&clipRect);
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		else if(brow && rowspliter > 0)
		{
			pT->m_bDragColSpliter = false;
			pT->m_bDragRowSpliter = true;
			pT->m_nSpliterIndex = rowspliter;
			pT->m_nPositionOrigin = pT->m_vnRowSpliter[rowspliter];

			clipRect.top = pT->m_vnRowSpliter[rowspliter-1]+40;
			clipRect.bottom = pT->m_vnRowSpliter[rowspliter+1]-40;
			ClientToScreen(hWnd,(LPPOINT)&clipRect);
			ClientToScreen(hWnd,(LPPOINT)&clipRect+1);
			ClipCursor(&clipRect);
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		pT->m_ptOrigin = pT->m_ptCurr = point;
		
		ReleaseDC(hWnd, hDC);
		return true;
	}

	// deal with title rect
	RECT titleRect = pT->GetTitleRect(hDC);
	ReleaseDC(hWnd, hDC);

	bool bSel;
	if(PtInRect(&titleRect, point))
	{
		bSel = pT->IsSelectTitle();
		pT->UnselectAll();
		pT->SetSelectTitle(!bSel);
		return true;
	}

	int idx = pT->GetPlotIndex( point );
	if( idx < 0 ) return false;

	bSel = pT->GetPlot(idx)->IsPlotSelected();
	pT->UnselectAll();
	pT->GetPlot(idx)->SetPlotSelected(!bSel);
	
	return pT->m_vpPlots[idx]->OnLButtonDown(hWnd, point);
}

template <typename SplitPlotImplT>
bool	CSplitHandler<SplitPlotImplT>::OnLButtonUp( HWND hWnd, POINT point )
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);

	pT->m_bDragColSpliter = false;
	pT->m_bDragRowSpliter = false;

	int idx = pT->GetPlotIndex( point );
	if( idx < 0 ) return false;
	return pT->m_vpPlots[idx]->OnLButtonUp(hWnd, point);
}

template <typename SplitPlotImplT>
bool	CSplitHandler<SplitPlotImplT>::OnLButtonDblClk( HWND hWnd, POINT point )
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;
	// deal with title rect

	int idx = pT->GetPlotIndex( point );
	if( idx < 0 ) return false;
	return pT->m_vpPlots[idx]->OnLButtonDblClk(hWnd, point);
}

template <typename SplitPlotImplT>
bool	CSplitHandler<SplitPlotImplT>::OnMouseMove( HWND hWnd, POINT point )
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	// deal with title rect
	HDC hDC = GetDC( hWnd );
	RECT titleRect = pT->GetTitleRect(hDC);
	ReleaseDC(hWnd, hDC);
	if(PtInRect(&titleRect, point))
	{
		pT->UnlightAll();
		pT->SetLightTitle(true);
		return true;
	}
	else
	{
		pT->SetLightTitle(false);
	}
	
	// Drag the edge
	if(pT->m_bDragColSpliter)
	{
		pT->m_ptCurr = point;
		
		int pos = pT->m_nPositionOrigin;
		pos+=(pT->m_ptCurr.x-pT->m_ptOrigin.x);
		pT->m_vnColSpliter[pT->m_nSpliterIndex] = pos;

		pT->CalcColOffsets();
		return true;
	}
	else if(pT->m_bDragRowSpliter)
	{
		pT->m_ptCurr = point;
		
		int pos = pT->m_nPositionOrigin;
		pos+=(pT->m_ptCurr.y-pT->m_ptOrigin.y);
		pT->m_vnRowSpliter[pT->m_nSpliterIndex] = pos;

		pT->CalcRowOffsets();
		return true;
	}

	// SetCursor
	int spliter;
	if( pT->HitColSpliter(point, spliter) )
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
	else if( pT->HitRowSpliter(point, spliter) )
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	}
	
	// default behaver
	int idx = pT->GetPlotIndex( point );
	if( idx < 0 ) return false;
	pT->UnlightAll();
	pT->m_vpPlots[idx]->SetPlotLighted(true);
	return pT->m_vpPlots[idx]->OnMouseMove(hWnd, point);
}

template <typename SplitPlotImplT>
bool	CSplitHandler<SplitPlotImplT>::OnContextMenu( HMENU hMenu, HWND hWnd, POINT point )
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);
	POINT pt = point;
	ScreenToClient(hWnd, &pt);
	if(!PtInRect(&pT->GetLastClientRect(), pt))return false;

	int idx = pT->GetPlotIndex( pt );
	if( idx < 0 ) return false;
	return pT->m_vpPlots[idx]->OnContextMenu(hMenu, hWnd, point);
}

template <typename SplitPlotImplT>
bool	CSplitHandler<SplitPlotImplT>::OnKeyDown( HWND hWnd, UINT key )
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);

	int idx;
	bool needUpdate = false;
	switch(key)
	{
	case VK_TAB:
		pT->SwitchSelection();
		needUpdate = true;
		break;
	case VK_SPACE:
	case VK_RETURN:
		idx = pT->GetIndexOfSelectedSubplot();
		if( idx < 0 ) return false;
		needUpdate = pT->m_vpPlots[idx]->OnKeyDown(hWnd, key);
	}
	
	return needUpdate;
}

template <typename SplitPlotImplT>
void	CSplitHandler<SplitPlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SplitPlotImplT *pT=static_cast<SplitPlotImplT*>(this);
	POINT	point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	UINT	key = wParam;
	switch(message)
	{
	case WM_LBUTTONDOWN:
		if(OnLButtonDown(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_LBUTTONUP:
		if(OnLButtonUp(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_LBUTTONDBLCLK:
		if(OnLButtonDblClk(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_MOUSEMOVE:
		if(OnMouseMove(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_CONTEXTMENU:
		if(OnContextMenu(NULL, hWnd, point))pT->OnDraw(hWnd);
		break;
	case WM_KEYDOWN:
		pT->OnKeyDown(hWnd, key);
		break;
	default:
		return;
	}
}

#endif