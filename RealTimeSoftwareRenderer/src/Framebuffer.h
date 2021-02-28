#pragma once

#include <windows.h>

#include "Vector.h"

class Framebuffer
{
public:
	static const int PIXEL_PER_BYTE = 3;
	static const int PIXEL_PER_BIT = 24;

public:
	Framebuffer(HWND hWnd);
	virtual ~Framebuffer(void);

	void Render(void);
	void Clear(BYTE color[3]);

	void SetPixel(int x, int y, BYTE color[3]);
	void SetPixel(int x, int y, const RGB24& RGBColor);

	bool GetPixel(int x, int y, BYTE color[3]);
	bool GetPixel(int x, int y, RGB24& RGBColor);

private:
	void CreateFramebuffer(void);
	void DestroyFramebuffer(void);

	void SetFramebufferSize(void);
	void SetFramebufferByteScanline(void);
	void SetFramebufferHDC(void);
	void SetFramebufferDIBSection(void);

	bool IsIncludePixelInFramebuffer(int x, int y);
	int  GetOffset(int x, int y);

private:
	HWND    m_hWnd;
	HDC     m_hScreenDC;
	HDC     m_hMemoryDC;
	HBITMAP m_hOldBitmap;
	HBITMAP m_hDIBitmap;
	BYTE*   m_pBuffer;
	int     m_iBufferWidth;
	int     m_iBufferHeight;
	int     m_iByteScanline;
};



