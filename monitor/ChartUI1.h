#pragma once
#include "stdafx.h"
#include "../Cchart/Plot/PlotInterface.h"

class CCharTUI: public CControlUI //,public CWindowWnd//继承CWindowWnd是为了设置Chart
{  
public:  
	CCharTUI();
	BOOL Attach(HWND hwnd);
	HWND Detach();

	//CControlUI
	virtual void SetInternVisible(bool bVisible = true);
	virtual void SetPos(RECT rc);

	//CWindowWnd
	//virtual LPCTSTR    GetWindowClassName() const   {   return _T("ChatUIBase");  }  
	//virtual UINT GetClassStyle() const{ return CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;}
	//virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:   
	HWND m_hWnd;
};  