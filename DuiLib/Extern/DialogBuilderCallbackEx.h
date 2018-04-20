#pragma once
#include "UILib.h"
class UILIB_API CDialogBuilderCallbackEx :
	public DuiLib::IDialogBuilderCallback {
public:
    CDialogBuilderCallbackEx(DuiLib::CPaintManagerUI *ppm = NULL);
    ~CDialogBuilderCallbackEx(void);

    virtual DuiLib::CControlUI *CreateControl(LPCTSTR pstrClass);

private:
    // ��һ��PAINTMANAGER��Ϊ�˼��ء�Default ��
    DuiLib::CPaintManagerUI *m_pm;
};

