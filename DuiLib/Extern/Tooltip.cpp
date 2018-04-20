#include "stdafx.h"
#include "Tooltip.h"

using namespace DuiLib;
Tooltip::Tooltip(void)
{

}

Tooltip::~Tooltip(void)
{

}

LPCTSTR Tooltip::GetWindowClassName() const
{
	return _T("Tooltip");
}


DuiLib::UILIB_RESOURCETYPE Tooltip::GetResourceType() const
{
	return DuiLib::UILIB_FILE;
}

DuiLib::CDuiString Tooltip::GetSkinFile()
{
	return _T("ResMain\\tooltip.xml");
}

DuiLib::CDuiString Tooltip::GetSkinFolder()
{
	return _T("");
}

void Tooltip::InitWindow()
{
	RECT rectWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE); 
	LONG	cx = rectWorkArea.right-rectWorkArea.left;//GetSystemMetrics (SM_CXSCREEN);
	LONG	cy = rectWorkArea.bottom-rectWorkArea.top;// GetSystemMetrics (SM_CYSCREEN);
	::SetWindowPos(m_hWnd, HWND_TOPMOST,rectWorkArea.left, rectWorkArea.top, cx, cy, SWP_NOREPOSITION|SWP_NOSIZE|SWP_SHOWWINDOW);
	ResizeClient(cx,cy);
	cx =::GetSystemMetrics (SM_CXSCREEN);
	cy = GetSystemMetrics (SM_CYSCREEN);
	//m_PaintManager.SetInitSize(cx,cy);
	DuiLib::CVerticalLayoutUI* pVer = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("bgImage")));
	ASSERT(pVer);	
	//if(cx==1280 && cy==1024)
	//	pVer->SetBkImage(L"ResMain\\tooltip_bg_1280_1024.png");
	//else  if(cx==1600 && cy==900)
	//	pVer->SetBkImage(L"ResMain\\tooltip_bg_1600_900.png");
	//else  if(cx==1366 && cy==768)
	//	pVer->SetBkImage(L"ResMain\\tooltip_bg_1366_768.png");
	//else
		pVer->SetBkImage(L"ResMain\\tooltip_bg.png");
		
	DuiLib::CVerticalLayoutUI* pVer1 = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verBg1")));
	ASSERT(pVer1);
	DuiLib::CVerticalLayoutUI* pVer2 = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verBg2")));
	ASSERT(pVer1);
	if (cx <= 1024 || cy <= 768)
	{
		pVer1->SetVisible(false);
		pVer2->SetVisible(true);
	}
	else
	{
		pVer1->SetVisible(true);
		pVer2->SetVisible(false);
	}
}

void Tooltip::Notify(DuiLib::TNotifyUI& msg)
{
	if (msg.sType==DUI_MSGTYPE_CLICK &&	_tcsicmp(msg.pSender->GetName(), L"btnClose") == 0)
	{
			this->Close();
	}
}
