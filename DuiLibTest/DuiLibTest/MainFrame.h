#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include "UITimeScaleBar.h"

class CMainFrame : public WindowImplBase
{
public:
	CMainFrame(){}
	~CMainFrame(){}

public:
	virtual CDuiString GetSkinFolder(){
		return _T("");
	}
	virtual CDuiString GetSkinFile(){
		return _T("MainFrame.xml");
	}
	virtual LPCTSTR GetWindowClassName() const{
		return _T("DuiLibTestMainFrame");
	}
	virtual CControlUI* CreateControl(LPCTSTR pstrClass){
		if (_tcscmp(pstrClass, _T("TimeScaleBar")) == 0)
			return new CTimeScaleBarUI;
		return NULL;
	}
	virtual void OnFinalMessage(HWND hWnd){
		PostQuitMessage(555);
		delete this;
	}
};

#endif // __MAIN_FRAME_H__