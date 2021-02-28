#include "Framebuffer.h"

Framebuffer::Framebuffer(HWND hWnd) 
	: m_hWnd(hWnd)
	, m_hScreenDC(NULL), m_hMemoryDC(NULL)
	, m_hOldBitmap(NULL), m_hDIBitmap(NULL)
	, m_pBuffer(nullptr)
	, m_iBufferWidth(0), m_iBufferHeight(0), m_iByteScanline(0)
{
	CreateFramebuffer();
}

Framebuffer::~Framebuffer()
{
	DestroyFramebuffer();
}

void Framebuffer::SetFramebufferSize(void)
{
	RECT windowRect;
	GetClientRect(m_hWnd, &windowRect);

	m_iBufferWidth  = windowRect.right - windowRect.left;
	m_iBufferHeight = windowRect.bottom - windowRect.top;
}

void Framebuffer::SetFramebufferByteScanline(void)
{
	m_iByteScanline = (m_iBufferWidth * PIXEL_PER_BYTE + 3) & ~3;
}

void Framebuffer::SetFramebufferHDC(void)
{
	m_hScreenDC = GetDC(m_hWnd);
	m_hMemoryDC = CreateCompatibleDC(m_hScreenDC);
}

void Framebuffer::SetFramebufferDIBSection(void)
{
	BITMAPINFO bitmapInfo;
	ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

	bitmapInfo.bmiHeader.biSize        = sizeof(BITMAPINFO);
	bitmapInfo.bmiHeader.biWidth       = static_cast<LONG>(m_iBufferWidth);
	bitmapInfo.bmiHeader.biHeight      = static_cast<LONG>(m_iBufferHeight);
	bitmapInfo.bmiHeader.biBitCount    = PIXEL_PER_BIT;
	bitmapInfo.bmiHeader.biPlanes      = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	m_hDIBitmap = CreateDIBSection(
		m_hMemoryDC,
		&bitmapInfo,
		DIB_RGB_COLORS,
		(LPVOID*)&(m_pBuffer),
		NULL,
		NULL
	);

	m_hOldBitmap = (HBITMAP)SelectObject(
		m_hMemoryDC,
		m_hDIBitmap
	);
}

void Framebuffer::CreateFramebuffer(void)
{
	SetFramebufferSize();
	SetFramebufferByteScanline();
	SetFramebufferHDC();
	SetFramebufferDIBSection();
}

void Framebuffer::DestroyFramebuffer(void)
{
	SelectObject(m_hScreenDC, m_hOldBitmap);
	DeleteObject(m_hDIBitmap);
	ReleaseDC(m_hWnd, m_hMemoryDC);
}

void Framebuffer::Render(void)
{
	BitBlt(
		m_hScreenDC, 0, 0, m_iBufferWidth, m_iBufferHeight,
		m_hMemoryDC, 0, 0, SRCCOPY
	);
}

bool Framebuffer::IsIncludePixelInFramebuffer(int x, int y)
{
	return (0 <= x && x < m_iBufferWidth) && (0 <= y && y < m_iBufferHeight);
}

int Framebuffer::GetOffset(int x, int y)
{
	return x * PIXEL_PER_BYTE + m_iByteScanline * y;
}

void Framebuffer::Clear(BYTE color[3])
{
	int offset = 0;

	while (offset < m_iByteScanline)
	{
		*(m_pBuffer + offset + 0) = color[2];
		*(m_pBuffer + offset + 1) = color[1];
		*(m_pBuffer + offset + 2) = color[0];

		offset += PIXEL_PER_BYTE;
	}

	for (int col = 1, offset = m_iByteScanline; col < m_iBufferHeight; ++col, offset += m_iByteScanline)
	{
		memcpy(
			m_pBuffer + offset,
			m_pBuffer,
			m_iByteScanline
		);
	}
}

void Framebuffer::SetPixel(int x, int y, BYTE color[3])
{
	if (IsIncludePixelInFramebuffer(x, y))
	{
		int offset = GetOffset(x, y);

		*(m_pBuffer + offset + 0) = color[2];
		*(m_pBuffer + offset + 1) = color[1];
		*(m_pBuffer + offset + 2) = color[0];
	}
}

void Framebuffer::SetPixel(int x, int y, const RGB24& RGBColor)
{
	BYTE color[3];

	color[0] = static_cast<unsigned char>(255.0f * RGBColor.r);
	color[1] = static_cast<unsigned char>(255.0f * RGBColor.g);
	color[2] = static_cast<unsigned char>(255.0f * RGBColor.b);

	SetPixel(x, y, color);
}


bool Framebuffer::GetPixel(int x, int y, BYTE color[3])
{
	if (IsIncludePixelInFramebuffer(x, y))
	{
		int offset = GetOffset(x, y);

		color[2] = *(m_pBuffer + offset + 0);
		color[1] = *(m_pBuffer + offset + 1);
		color[0] = *(m_pBuffer + offset + 2);

		return true;
	}
	else
	{
		return false;
	}
}

bool Framebuffer::GetPixel(int x, int y, RGB24& RGBColor)
{
	BYTE color[3];

	if (GetPixel(x, y, color))
	{
		RGBColor.r = static_cast<float>(color[0]) / 255.0f;
		RGBColor.g = static_cast<float>(color[1]) / 255.0f;
		RGBColor.b = static_cast<float>(color[2]) / 255.0f;

		return true;
	}
	else
	{
		return false;
	}
}