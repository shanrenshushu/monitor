#pragma once

namespace DuiLib
{
	class UILIB_API UIMessageBox : public CWindowWnd,public IMessageFilterUI
	{
	public:
		UIMessageBox();
		~UIMessageBox();
		enum WhichControl
		{
			eLeftBtn,
			eRightBtn,
			eEdit,
		};
		enum IconStyle
		{
			eCuo,
			eDui,
			eTan,
			eWen,
		};

	public:
		virtual LPCTSTR GetWindowClassName()const;
		virtual void	OnFinalMessage(HWND hWnd){delete this;}
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		virtual void OnInit();

		bool EventHandle(void *msg);
		void KeyAndClick(CControlUI *pSender);

		/*******************
		caption：标题，
		text:内容，
		style：类型，取值：MB_OK（确认），MB_OKCANCEL（取消，确认），MB_YESNO （否，是）
		返回值：
		IDOK = 1　　　　　　　　//按确定按钮
		IDCANCEL = 2　　　　　　//按取消按钮
		IDYES = 6　　　　　　　 //按是按钮
		IDNO = 7　　　　　　　　//按否按钮
		*******************/
		void Create(HWND parent,LPCTSTR text,LPCTSTR caption=L"提示",UINT style = MB_OK,IconStyle icon =eDui);

		//特殊情况下，修改各个控件的属性接口
		void SetAttribute(WhichControl which,LPCTSTR pstrName, LPCTSTR pstrValue);
	private:
		CPaintManagerUI	 m_pm;
		UINT mStyle;

		CLabelUI *mpCaption;
		CLabelUI *mpIcon;
		CButtonUI *mpClose;
		CButtonUI *mpLeftBtn;
		CButtonUI *mpRightBtn;
		CRichEditUI *mpRichEdit;
	};
}