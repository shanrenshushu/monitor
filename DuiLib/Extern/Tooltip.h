#pragma once
namespace DuiLib
{
	class UILIB_API Tooltip : public DuiLib::WindowImplBase
	{
	public:
		Tooltip(void);
		~Tooltip(void);
	public:
		LPCTSTR GetWindowClassName() const;	
		virtual void OnFinalMessage(HWND hWnd)
		{
			delete this;
		};
		virtual void InitWindow();
		virtual DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
		//virtual LPCTSTR GetResourceID() const;
		virtual DuiLib::CDuiString GetSkinFile();
		virtual DuiLib::CDuiString GetSkinFolder();
	protected:
		void Notify(DuiLib::TNotifyUI& msg);
	};
}