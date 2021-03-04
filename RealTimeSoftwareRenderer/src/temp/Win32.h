#pragma once

#include <windows.h>

class Win32App
{
public:
	Win32App();
	virtual ~Win32App();

	bool InitializeWin32App(LPCWSTR title, int width, int height);
	
	HWND GetHWND(void) { return m_hWnd; }

private:
	bool SetWin32AppTitle(LPCWSTR title);
	bool SetWin32AppWindowClass(void);

private:
	LPWSTR    m_sTitle;
	int       m_iWndWidth;
	int       m_iWndHeight;
	HWND      m_hWnd;
	HINSTANCE m_hInstance;
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
