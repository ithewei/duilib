#include "stdafx.h"
#include "MainFrame.h"

int _stdcall WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd ){
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\DuiLibTestRes"));
	//CPaintManagerUI::SetResourceZip(_T("skin.zip"));

	CMainFrame* pFrame = new CMainFrame;
	pFrame->Create(NULL, _T("DuiLibTest"), UI_WNDSTYLE_FRAME, 0);
	pFrame->CenterWindow();
	pFrame->ShowWindow();

	CPaintManagerUI::MessageLoop();

	return 0;
}