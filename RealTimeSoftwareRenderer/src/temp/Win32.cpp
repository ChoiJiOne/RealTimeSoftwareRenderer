#include "Win32.h"
#include <iostream>

Win32App::Win32App()
	: m_sTitle(nullptr)
	, m_iWndWidth(0), m_iWndHeight(0)
	, m_hWnd(NULL), m_hInstance(NULL)
{

}

Win32App::~Win32App()
{
	UnregisterClass(m_sTitle, m_hInstance);

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	if (m_sTitle != nullptr)
	{
		delete[] m_sTitle;
		m_sTitle = nullptr;
	}
}

bool Win32App::SetWin32AppTitle(LPCWSTR title)
{
	m_sTitle = new WCHAR[wcslen(title) + 1];

	if (m_sTitle == nullptr)
	{
		return false;
	}
	else
	{
		wcscpy(m_sTitle, title);
		return true;
	}
}

bool Win32App::SetWin32AppWindowClass(void)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDC_ICON);
	wc.hInstance     = m_hInstance;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.lpszClassName = m_sTitle;
	wc.lpszMenuName  = nullptr;
	wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	return RegisterClassEx(&wc) != 0;
}

bool Win32App::InitializeWin32App(LPCWSTR title, int width, int height)
{
	m_iWndWidth  = width;
	m_iWndHeight = height;

	SetWin32AppTitle(title);
	SetWin32AppWindowClass();

	int posX = (GetSystemMetrics(SM_CXSCREEN) - m_iWndWidth)  / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - m_iWndHeight) / 2;

	m_hWnd = CreateWindowEx(
		0, m_sTitle, m_sTitle,
		WS_POPUP,
		posX, posY, m_iWndWidth, m_iWndHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (m_hWnd != NULL)
	{
		ShowWindow(m_hWnd, SW_SHOW);
		return true;
	}
	else
	{
		return false;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
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