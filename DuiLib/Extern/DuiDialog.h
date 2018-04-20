// DuiDialog.h: interface for the CDuiDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUIDIALOG_H__F0608F4A_CEA2_44BE_9F21_F5FAFD3B2454__INCLUDED_)
#define AFX_DUIDIALOG_H__F0608F4A_CEA2_44BE_9F21_F5FAFD3B2454__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace DuiLib
{
	class UILIB_API CDuiDialog  : public DuiLib::CWindowWnd, public DuiLib::INotifyUI 
	{
	public:
		CDuiDialog();
		virtual ~CDuiDialog();

		HWND Create (LPCTSTR lpszUISkin, DuiLib::IDialogBuilderCallback *pCallback = NULL);
		HWND Create( LPCTSTR lpszUISkin, DuiLib::IDialogBuilderCallback *pCallback, DWORD dwStyle, DWORD dwExStyle /*=0*/);

		virtual LPCTSTR GetWindowClassName() const 
		{
			return _T("UIDialog");
		};
		UINT GetClassStyle() const 
		{
			return UI_CLASSSTYLE_DIALOG;
		};

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void OnFinalMessage(HWND /*hWnd*/) 
		{
			delete this;
		};
		virtual void OnInit ();
		virtual void OnNotify(DuiLib::TNotifyUI &msg);
		virtual void OnClick (DuiLib::CControlUI *pSender);
		virtual void OnSelChanged( DuiLib::CControlUI *pSender );
		virtual void OnKillFocus (DuiLib::CControlUI *pSender);

		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual UINT MessageBox( LPCTSTR lpText, UINT uIcon = MB_ICONWARNING , LPCTSTR lpCaption = _T("Ã· æ"), UINT uType = MB_OK,LPCTSTR lpBtnYesText=_T(" «"),LPCTSTR lpBtnNoText=_T("∑Ò") );

		DWORD GetColor(LPCTSTR pszColorValue);

	protected:
		void Notify(DuiLib::TNotifyUI &msg);

	public:
		DuiLib::CPaintManagerUI m_pm;
		DuiLib::CDuiString		m_strUISkin;
		DuiLib::IDialogBuilderCallback *m_pCallback;
	};
}

#endif // !defined(AFX_DUIDIALOG_H__F0608F4A_CEA2_44BE_9F21_F5FAFD3B2454__INCLUDED_)
