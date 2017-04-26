#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

// 窗口过程
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{
	case WM_LBUTTONDOWN:
		MessageBox(hWnd, "你单击了鼠标左键", "WM_LBUTTONDOWN", MB_OK);
		break;
	case WM_CHAR:
		char szChar[64];
		sprintf_s(szChar, 64, "你按下了键盘键:%c", wParam);
		MessageBox(hWnd, szChar, "WM_CHAR", MB_OK);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此添加任意绘图代码...
			SetTextColor(hdc, RGB(255,0,0));
			SetBkColor(hdc, RGB(0,255,0));
			TextOut(hdc, 200, 200, "Hello World!", strlen("Hello World!"));
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine, int       nCmdShow){
	// 注册窗口类
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground	= (HBRUSH)(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "mywndclass";
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);

	// 创建窗口
	HWND hWnd = CreateWindowEx(0, "mywndclass", "This is a win32 wnd", WS_OVERLAPPEDWINDOW,
		100, 100, 800, 600, NULL, NULL, hInstance, NULL);

	// 显示窗口
	ShowWindow(hWnd, nCmdShow);

	// 窗口消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}