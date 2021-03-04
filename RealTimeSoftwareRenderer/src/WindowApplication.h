#pragma once

#include <windows.h>

class WindowApplication
{
public:
	WindowApplication();
	virtual ~WindowApplication();

public:
	bool InitializeWindowApplication(LPCWSTR sWndTitle, int iWndWidth, int iWndHeight, bool isRemoveTitleBar = false, bool isFullScreen = false);
	void ShowWindowApplication(bool isShow);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

protected:
	void SetWindowApplicationInstance(void);
	void SetWindowApplicationTitle(LPCWSTR title);
	void SetWindowApplicationSize(int width, int height);
	bool SetWindowApplicationClass(WNDCLASSEX* wc);
	void SetWindowApplicationPosition(int& posX, int& posY);
	void SetWindowApplicationFullScreen(void);
	
	bool CreateWindowApplication(bool isRemoveTitleBar, bool isFullScreen);

	bool ResizeWindowApplication(void);
	bool RenameWindowApplicationTitle(LPCWSTR renameTitle);

protected:
	LPWSTR     m_sWndTitle;
	int        m_iWndWidth;
	int        m_iWndHeight;
	HWND       m_hWnd;
	HINSTANCE  m_hInstance;
	bool       m_bIsFullScreen;
	bool       m_bIsInitialize;
};