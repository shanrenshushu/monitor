#ifndef __UI_LABEL_EX_H__  
#define __UI_LABEL_EX_H__

namespace DuiLib
{
	class UILIB_API CLabelUIEx :
		public DuiLib::CLabelUI 
	{
	public:
		CLabelUIEx(void);
		~CLabelUIEx(void);

		virtual void PaintText(HDC hDC);  
		virtual void SetPos(RECT rc);
		void DoEvent(TEventUI& event);
		UINT GetControlFlags() const;

	private:  
		Color _MakeRGB(int a, Color cl);  
		Color _MakeRGB(int r, int g, int b); 
	};
}
#endif
