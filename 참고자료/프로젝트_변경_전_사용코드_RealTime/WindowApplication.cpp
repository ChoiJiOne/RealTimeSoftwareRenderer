#include <wchar.h>
#include "WindowApplication.h"

WindowApplication::WindowApplication()
	: m_sWndTitle(nullptr), m_iWndWidth(0), m_iWndHeight(0)
	, m_hWnd(NULL), m_hInstance(NULL)
	, m_bIsFullScreen(false)
	, m_bIsInitialize(false)
{
}

WindowApplication::~WindowApplication()
{
	if (m_bIsFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	UnregisterClass(m_sWndTitle, m_hInstance);

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	if (m_sWndTitle != nullptr)
	{
		delete[] m_sWndTitle;
		m_sWndTitle = nullptr;
	}
}

bool WindowApplication::InitializeWindowApplication(LPCWSTR sWndTitle, int iWndWidth, int iWndHeight, bool isRemoveTitleBar, bool isFullScreen)
{
	SetWindowApplicationInstance();
	SetWindowApplicationSize(iWndWidth, iWndHeight);
	SetWindowApplicationTitle(sWndTitle);

	if (CreateWindowApplication(isRemoveTitleBar, isFullScreen))
	{
		m_bIsInitialize = true;
	}

	return m_bIsInitialize;
}

void WindowApplication::ShowWindowApplication(bool isShow)
{
	ShowWindow(m_hWnd, isShow ? SW_SHOW : SW_HIDE);
}

void WindowApplication::SetWindowApplicationInstance(void)
{
	m_hInstance = GetModuleHandle(NULL);
}

void WindowApplication::SetWindowApplicationTitle(LPCWSTR title)
{
	if (m_sWndTitle != nullptr)
	{
		delete[] m_sWndTitle;
		m_sWndTitle = nullptr;
	}

	m_sWndTitle = new WCHAR[wcslen(title) + 1];
	wcscpy(m_sWndTitle, title);
}

void WindowApplication::SetWindowApplicationSize(int width, int height)
{
	m_iWndWidth  = width;
	m_iWndHeight = height;
}

bool WindowApplication::SetWindowApplicationClass(WNDCLASSEX* wc)
{
	memset(wc, 0, sizeof(WNDCLASSEX));

	wc->cbSize        = sizeof(WNDCLASSEX);
	wc->cbClsExtra    = 0;
	wc->cbWndExtra    = 0;
	wc->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc->hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc->hIcon         = LoadIcon(NULL, IDC_ICON);
	wc->hInstance     = m_hInstance;
	wc->lpfnWndProc   = (WNDPROC)WndProc;
	wc->lpszClassName = m_sWndTitle;
	wc->lpszMenuName  = nullptr;
	wc->style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	return RegisterClassEx(wc) != 0;
}

void WindowApplication::SetWindowApplicationPosition(int& posX, int& posY)
{
	posX = (GetSystemMetrics(SM_CXSCREEN) - m_iWndWidth)  / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - m_iWndHeight) / 2;
}

void WindowApplication::SetWindowApplicationFullScreen(void)
{
	m_bIsFullScreen = true;
	m_iWndWidth = GetSystemMetrics(SM_CXSCREEN);
	m_iWndHeight = GetSystemMetrics(SM_CYSCREEN);

	DEVMODE dmScreenSettings;
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));

	dmScreenSettings.dmSize = sizeof(DEVMODE);
	dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(m_iWndWidth);
	dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(m_iWndHeight);
	dmScreenSettings.dmBitsPerPel = 24;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
}

bool WindowApplication::CreateWindowApplication(bool isRemoveTitleBar, bool isFullScreen)
{
	WNDCLASSEX wc;
	int posX = 0;
	int posY = 0;

	if (!SetWindowApplicationClass(&wc))
	{
		return false;
	}

	if (isFullScreen)
	{
		SetWindowApplicationFullScreen();
	}

	SetWindowApplicationPosition(posX, posY);

	m_hWnd = CreateWindowEx(
		0, m_sWndTitle, m_sWndTitle,
		isRemoveTitleBar ? WS_POPUP : WS_OVERLAPPEDWINDOW,
		posX, posY, m_iWndWidth, m_iWndHeight,
		NULL,
		NULL,
		m_hInstance,
		nullptr
	);

	return m_hWnd != NULL;
}

bool WindowApplication::ResizeWindowApplication(void)
{
	RECT nowWindowRect;
	bool isResizeWindow = false;

	GetClientRect(m_hWnd, &nowWindowRect);

	int nowWindowWidth  = nowWindowRect.right - nowWindowRect.left;
	int nowWindowHeight = nowWindowRect.bottom - nowWindowRect.top;

	if (!((nowWindowWidth == m_iWndWidth) && (nowWindowHeight == m_iWndHeight)))
	{
		SetWindowApplicationSize(nowWindowWidth, nowWindowHeight);
		isResizeWindow = true;
	}

	return isResizeWindow;
}

bool WindowApplication::RenameWindowApplicationTitle(LPCWSTR renameTitle)
{
	SetWindowApplicationTitle(renameTitle);
	return SetWindowText(m_hWnd, m_sWndTitle);
}

LRESULT CALLBACK WindowApplication::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return 0;
}
