#include <cfloat>
#include "DIBSection.h"

DIBSection::DIBSection(HWND hWnd, bool isCheckRange)
	: m_hWnd(hWnd)
	, m_hScreenDC(NULL), m_hMemoryDC(NULL), m_hDIBitmap(NULL), m_hOldBitmap(NULL), m_pBuffer(nullptr), m_pZBuffer(nullptr)
	, m_iWndWidth(0), m_iWndHeight(0), m_iByteScanLine(0), m_bIsCheckRange(isCheckRange)
{
	GenerateDIBSection();
}

DIBSection::~DIBSection()
{
	DestroyDIBSection();
}

void DIBSection::SetWindowSize(void)
{
	RECT nowWindowRect;
	GetClientRect(m_hWnd, &nowWindowRect);

	m_iWndWidth  = nowWindowRect.right  - nowWindowRect.left;
	m_iWndHeight = nowWindowRect.bottom - nowWindowRect.top;
}

void DIBSection::SetByteScanLine(void)
{
	m_iByteScanLine = (m_iWndWidth * PIXEL_PER_BYTE + 3) & ~3;
}

void DIBSection::InitializeBITMAPINFO(BITMAPINFO* pBITMAPINFO)
{
	memset(pBITMAPINFO, 0, sizeof(BITMAPINFO));

	pBITMAPINFO->bmiHeader.biSize     = sizeof(BITMAPINFO);
	pBITMAPINFO->bmiHeader.biWidth    = static_cast<LONG>(m_iWndWidth);
	pBITMAPINFO->bmiHeader.biHeight   = static_cast<LONG>(m_iWndHeight);
	pBITMAPINFO->bmiHeader.biBitCount = PIXEL_PER_BIT;
	pBITMAPINFO->bmiHeader.biPlanes   = 1;
	pBITMAPINFO->bmiHeader.biCompression = BI_RGB;
}

void DIBSection::InitializeHDC(void)
{
	m_hScreenDC = GetDC(m_hWnd);
	m_hMemoryDC = CreateCompatibleDC(m_hScreenDC);
}

void DIBSection::InitializeDIBSection(BITMAPINFO* pBITMAPINFO)
{
	m_hDIBitmap = CreateDIBSection(
		m_hMemoryDC,
		pBITMAPINFO,
		DIB_RGB_COLORS,
		(LPVOID*)(&m_pBuffer),
		NULL,
		NULL
	);

	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemoryDC, m_hDIBitmap);
}

void DIBSection::CreateZBuffer(void)
{
	DestroyZBuffer();

	m_pZBuffer = new float[m_iWndWidth * m_iWndHeight];
}

void DIBSection::DestroyZBuffer(void)
{
	if (m_pZBuffer != nullptr)
	{
		delete[] m_pZBuffer;
		m_pZBuffer = nullptr;
	}
}

void DIBSection::GenerateDIBSection(void)
{
	DestroyDIBSection();
	DestroyZBuffer();

	BITMAPINFO bitmapInfo;

	SetWindowSize();
	SetByteScanLine();

	InitializeHDC();
	InitializeBITMAPINFO(&bitmapInfo);
	InitializeDIBSection(&bitmapInfo);

	CreateZBuffer();
}

void DIBSection::DestroyDIBSection(void)
{
	if (m_pBuffer != nullptr)
	{
		SelectObject(m_hScreenDC, m_hOldBitmap);
		DeleteObject(m_hDIBitmap);
		ReleaseDC(m_hWnd, m_hMemoryDC);

		m_pBuffer = nullptr;

		DestroyZBuffer();
	}
}

bool DIBSection::IsInsidePixel(int x, int y)
{
	return (0 <= x && x < m_iWndWidth) && (0 <= y && y < m_iWndHeight);
}

int DIBSection::GetOffset(int x, int y)
{
	return (PIXEL_PER_BYTE * x + m_iByteScanLine * y);
}

int  DIBSection::GetZOffset(int x, int y)
{
	return x + y * m_iWndWidth;
}

void DIBSection::Clear(BYTE red, BYTE green, BYTE blue)
{
	int offset = 0;

	while (offset < m_iByteScanLine)
	{
		*(m_pBuffer + offset + 0) = blue;
		*(m_pBuffer + offset + 1) = green;
		*(m_pBuffer + offset + 2) = red;

		offset += PIXEL_PER_BYTE;
	}

	offset = m_iByteScanLine;

	for (int col = 1; col < m_iWndHeight; ++col, offset += m_iByteScanLine)
	{
		memcpy(m_pBuffer + offset, m_pBuffer, m_iByteScanLine);
	}
}

void DIBSection::Clear(float red, float green, float blue)
{
	BYTE r = static_cast<BYTE>(255.0f * red  );
	BYTE g = static_cast<BYTE>(255.0f * green);
	BYTE b = static_cast<BYTE>(255.0f * blue );

	Clear(r, g, b);
}

void DIBSection::ClearZBuffer(void)
{
	for (int i = 0; i < m_iWndWidth * m_iWndHeight; ++i)
	{
		m_pZBuffer[i] = -FLT_MAX;
	}
}

void DIBSection::Render(void)
{
	BitBlt(
		m_hScreenDC, 0, 0, m_iWndWidth, m_iWndHeight,
		m_hMemoryDC, 0, 0, SRCCOPY
	);
}

void DIBSection::SetPixel(int x, int y, BYTE red, BYTE green, BYTE blue)
{
	if (m_bIsCheckRange && !IsInsidePixel(x, y))
	{
		return;
	}

	int offset = GetOffset(x, y);

	*(m_pBuffer + offset + 0) = blue;
	*(m_pBuffer + offset + 1) = green;
	*(m_pBuffer + offset + 2) = red;
}

void DIBSection::SetPixel(int x, int y, float red, float green, float blue)
{
	BYTE r = static_cast<BYTE>(255.0f * red);
	BYTE g = static_cast<BYTE>(255.0f * green);
	BYTE b = static_cast<BYTE>(255.0f * blue);

	SetPixel(x, y, r, g, b);
}

void DIBSection::SetZBuffer(int x, int y, float z, unsigned char red, unsigned char green, unsigned char blue)
{
	if (IsInsidePixel(x, y))
	{
		int offset = GetZOffset(x, y);

		if (m_pZBuffer[offset] < z)
		{
			m_pZBuffer[offset] = z;
			SetPixel(x, y, red, green, blue);
		}
	}
}

void DIBSection::SetZBuffer(float u, float v, float z, unsigned char red, unsigned char green, unsigned char blue)
{
	int x = static_cast<int>(u * static_cast<float>(m_iWndWidth));
	int y = static_cast<int>(v * static_cast<float>(m_iWndHeight));

	SetZBuffer(x, y, z, red, green, blue);
}