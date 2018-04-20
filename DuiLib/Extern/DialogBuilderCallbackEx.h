#pragma once
#include "UILib.h"
class UILIB_API CDialogBuilderCallbackEx :
	public DuiLib::IDialogBuilderCallback {
public:
    CDialogBuilderCallbackEx(DuiLib::CPaintManagerUI *ppm = NULL);
    ~CDialogBuilderCallbackEx(void);

    virtual DuiLib::CControlUI *CreateControl(LPCTSTR pstrClass);

private:
    // 加一个PAINTMANAGER是为了加载　Default 项
    DuiLib::CPaintManagerUI *m_pm;
};

