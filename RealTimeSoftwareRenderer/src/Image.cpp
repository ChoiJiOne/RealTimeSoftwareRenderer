#include <cstring>
#include <iostream>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include "Image.h"

Image::Image(const char* filePath, bool vertically) 
	: m_pBuffer(nullptr), m_bIsLoadImage(true), m_bIsVertically(vertically)
	, m_iChannels(0), m_iWidth(0), m_iHeight(0), m_iScanline(0)
{
	m_pBuffer   = stbi_load(filePath, &m_iWidth, &m_iHeight, &m_iChannels, 0);
	m_iScanline = m_iWidth * m_iChannels;

	if (m_pBuffer == nullptr)
	{
		throw std::runtime_error("Failed To Read Image");
	}
}

Image::Image(int width, int height, int channels, bool vertically, unsigned char red, unsigned char green, unsigned char blue)
	: m_pBuffer(nullptr), m_bIsLoadImage(false), m_bIsVertically(vertically)
	, m_iChannels(channels), m_iWidth(width), m_iHeight(height), m_iScanline(m_iWidth * m_iChannels)
{
	m_pBuffer = new unsigned char[m_iChannels * m_iWidth * m_iHeight];

	memset(m_pBuffer, 0, sizeof(unsigned char) * m_iChannels * m_iWidth * m_iHeight);
	ClearBuffer(red, green, blue);
}

Image::~Image()
{
	if (m_bIsLoadImage)
	{
		stbi_image_free(m_pBuffer);
	}
	else
	{
		delete[] m_pBuffer;
	}

	m_pBuffer = nullptr;
}

int Image::GetOffset(int x, int y)
{
	if (m_bIsVertically)
	{
		return (m_iHeight - 1 - y) * m_iScanline + x * m_iChannels;
	}
	else
	{
		return y * m_iScanline + x * m_iChannels;
	}
}

bool Image::IsIncludePixelInBuffer(int x, int y)
{
	return (0 <= x && x < m_iWidth) && (0 <= y && y < m_iHeight);
}

void Image::ClearBuffer(unsigned char red, unsigned char green, unsigned char blue)
{
	int offset;

	for (int col = 0; col < m_iWidth; ++col)
	{
		offset = m_iChannels * col;

		m_pBuffer[offset + 0] = red;
		m_pBuffer[offset + 1] = green;
		m_pBuffer[offset + 2] = blue;
	}

	for (int row = 1, offset = m_iScanline; row < m_iHeight; ++row, offset += m_iScanline)
	{
		memcpy(
			m_pBuffer + offset,
			m_pBuffer,
			m_iScanline
		);
	}
}

void Image::SetPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
	if (IsIncludePixelInBuffer(x, y))
	{
		int offset = GetOffset(x, y);
		
		m_pBuffer[offset + 0] = red;
		m_pBuffer[offset + 1] = green;
		m_pBuffer[offset + 2] = blue;
	}
}

void Image::SetPixel(float u, float v, unsigned char red, unsigned char green, unsigned char blue)
{
	int x = static_cast<int>(u * static_cast<float>(m_iWidth));
	int y = static_cast<int>(v * static_cast<float>(m_iHeight));
	
	SetPixel(x, y, red, green, blue);
}

bool Image::GetPixel(int x, int y, unsigned char& red, unsigned char& green, unsigned char& blue)
{
	if (IsIncludePixelInBuffer(x, y))
	{
		int offset = GetOffset(x, y);

		red   = m_pBuffer[offset + 0];
		green = m_pBuffer[offset + 1];
		blue  = m_pBuffer[offset + 2];

		return true;
	}
	else
	{
		return false;
	}
}

bool Image::GetPixel(float u, float v, unsigned char& red, unsigned char& green, unsigned char& blue)
{
	int x = static_cast<int>(u * static_cast<float>(m_iWidth));
	int y = static_cast<int>(v * static_cast<float>(m_iHeight));

	return GetPixel(x, y, red, green, blue);
}

bool Image::Save(const char* savePath)
{
	bool isSuccess = true;

	if (std::strstr(savePath, ".png") || std::strstr(savePath, ".PNG"))
	{
		stbi_write_png(savePath, m_iWidth, m_iHeight, m_iChannels, m_pBuffer, m_iScanline);
	}
	else if (std::strstr(savePath, ".jpg") || std::strstr(savePath, ".JPG"))
	{
		stbi_write_jpg(savePath, m_iWidth, m_iHeight, m_iChannels, m_pBuffer, 100);
	}
	else
	{
		isSuccess = false;
	}

	return isSuccess;
}