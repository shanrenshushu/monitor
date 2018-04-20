#pragma once

namespace DuiLib
{
	class UILIB_API CPopupWin : public CWindowWnd
	{
	public:
		virtual void Init(HWND hWndParent,const RECT &rect);
		virtual LPCTSTR GetWindowClassName() const;

		virtual void OnFinalMessage( HWND hWnd );

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)=0;
		virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		
	public:
		CPaintManagerUI m_PaintManager;
	};

}	// namespace DuiLib
