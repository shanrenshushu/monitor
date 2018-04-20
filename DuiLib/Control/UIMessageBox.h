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
		caption�����⣬
		text:���ݣ�
		style�����ͣ�ȡֵ��MB_OK��ȷ�ϣ���MB_OKCANCEL��ȡ����ȷ�ϣ���MB_YESNO �����ǣ�
		����ֵ��
		IDOK = 1����������������//��ȷ����ť
		IDCANCEL = 2������������//��ȡ����ť
		IDYES = 6�������������� //���ǰ�ť
		IDNO = 7����������������//����ť
		*******************/
		void Create(HWND parent,LPCTSTR text,LPCTSTR caption=L"��ʾ",UINT style = MB_OK,IconStyle icon =eDui);

		//��������£��޸ĸ����ؼ������Խӿ�
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