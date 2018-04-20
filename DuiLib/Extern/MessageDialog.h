#pragma once
enum eMessageType
{
	eType_Check = (1 << 8),//前8个是windows消息，且windows不支持异或操作比较，该枚举针对带checkbox的情况
	eType_NoCheck=(1 << 9),
};

#define MB_ICON_ERROR2								WM_USER + 109

namespace DuiLib
{
	class UILIB_API CMessageDialog : public DuiLib::CWindowWnd, public DuiLib::INotifyUI {
	public:
		CMessageDialog(void);
		~CMessageDialog(void);

		LPCTSTR GetWindowClassName() const;
		UINT GetClassStyle() const;
		void OnFinalMessage(HWND /*hWnd*/);

		void SetBkColor(DWORD dwBackColor);
		DWORD GetBkColor() const;
		void SetBkImage(DuiLib::CDuiString strBkImage);
		LPCTSTR GetBkImage();

		void Init();
		void Notify(DuiLib::TNotifyUI &msg);

		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

		UINT DuiMessageBox(HWND pWnd, LPCTSTR lpText, LPCTSTR lpCaption = _T("提示"), UINT uType = MB_OK, UINT uIcon = MB_ICONWARNING,LPCTSTR lpBtnYesText=_T("是"),LPCTSTR lpBtnNoText=_T("否"),DuiLib::CDuiString CheckBox=L"");
		static UINT DoMadol(HWND pWnd, LPCTSTR lpText, LPCTSTR lpCaption = _T("提示"), UINT uType = MB_OK, UINT uIcon = MB_ICONWARNING,LPCTSTR lpBtnYesText=_T("是"),LPCTSTR lpBtnNoText=_T("否"),DuiLib::CDuiString CheckBox=L"");
		//void DuiAboutBox(CWindowWnd *m_pMainWnd, LPCTSTR lpXMLPath = _T("ResMain\\about.xml"));

		void ShowCheckBox(const DuiLib::CDuiString &text);

	public:
		/* 主界面退出时, 隐藏的checkbox需要显示，从而把退出系统是否需要弹出提示框的结果写入配置文件
		* 该功能后续补充
		*/
		bool Remembered;  

	protected:
		DuiLib::CPaintManagerUI	 m_pm;
		DuiLib::CDuiString		 m_strXMLPath;
		DuiLib::CWindowWnd		*m_pMainWnd;
		UINT            m_uType;

		DuiLib::CButtonUI		*m_pButtonOK;
		DuiLib::CButtonUI		*m_pButtonCancel;
		DuiLib::CLabelUI        *m_pIconImg;
		DuiLib::CRichEditUI     *m_pRichEdit;
		DuiLib::CVerticalLayoutUI  *m_pTopContainer;
		DuiLib::CCheckBoxUI *mpCheckBox;
		UINT result;

		UINT uType;
	};
}


