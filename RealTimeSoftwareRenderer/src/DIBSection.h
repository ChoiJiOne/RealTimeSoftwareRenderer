#pragma once

#include <windows.h>

class DIBSection
{
public:
	static const int PIXEL_PER_BYTE = 3;
	static const int PIXEL_PER_BIT = 24;

public:
	DIBSection(HWND hWnd, bool isCheckRange = true);
	virtual ~DIBSection();

	void GenerateDIBSection(void);
	void DestroyDIBSection(void);

	void Clear(BYTE red, BYTE green, BYTE blue);
	void Clear(float red, float green, float blue);
	void ClearZBuffer(void);

	void SetPixel(int x, int y, BYTE red, BYTE green, BYTE blue);
	void SetPixel(int x, int y, float red, float green, float blue);

	void SetZBuffer(int x, int y, float z, unsigned char red, unsigned char green, unsigned char blue);
	void SetZBuffer(float u, float v, float z, unsigned char red, unsigned char green, unsigned char blue);
	
	void Render(void);

	HDC GetScreenDC(void) { return m_hScreenDC; }
	HDC GetMemoryDC(void) { return m_hMemoryDC; }
	int GetWidth(void)  const { return m_iWndWidth; }
	int GetHeight(void) const { return m_iWndHeight; }

private:
	void SetWindowSize(void);
	void SetByteScanLine(void);

	void InitializeBITMAPINFO(BITMAPINFO* pBITMAPINFO);
	void InitializeHDC(void);
	void InitializeDIBSection(BITMAPINFO* pBITMAPINFO);

	void CreateZBuffer(void);
	void DestroyZBuffer(void);

	int  GetOffset(int x, int y);
	int  GetZOffset(int x, int y);

	bool IsInsidePixel(int x, int y);

private:
	HWND    m_hWnd;
	HDC     m_hScreenDC;
	HDC     m_hMemoryDC;
	HBITMAP m_hDIBitmap;
	HBITMAP m_hOldBitmap;
	BYTE*   m_pBuffer;
	float*  m_pZBuffer;
	int     m_iWndWidth;
	int     m_iWndHeight;
	int     m_iByteScanLine;
	bool    m_bIsCheckRange;
};