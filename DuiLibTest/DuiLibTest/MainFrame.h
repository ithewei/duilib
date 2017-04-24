#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include "UITimeScaleBar.h"
#include "UIWnd.h"

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
		else if (_tcscmp(pstrClass, _T("Wnd")) == 0){
			CWndUI* pWndUI = new CWndUI;
			HWND    hWnd  = CreateWindow(_T("static"), _T("win32"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
			pWndUI->Attach(hWnd);
			return pWndUI;
		}
		return NULL;
	}
	virtual void OnFinalMessage(HWND hWnd){
		PostQuitMessage(555);
		delete this;
	}
};

#endif // __MAIN_FRAME_H__