#pragma once

class Image
{
public:
	Image(const char* filePath, bool vertically = true);
	Image(int width, int height, int channels, bool vertically = true, unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0);
	virtual ~Image();

	void SetPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
	void SetPixel(float u, float v, unsigned char red, unsigned char green, unsigned char blue);
	
	bool GetPixel(int x, int y, unsigned char& red, unsigned char& green, unsigned char& blue);
	bool GetPixel(float u, float v, unsigned char& red, unsigned char& green, unsigned char& blue);
	
	bool Save(const char* savePath);

	int GetWidth(void)  { return m_iWidth; }
	int GetHeight(void) { return m_iHeight; }

protected:
	int  GetOffset(int x, int y);
	bool IsIncludePixelInBuffer(int x, int y);
	void ClearBuffer(unsigned char red, unsigned char green, unsigned char blue);

protected:
	unsigned char* m_pBuffer;
	bool m_bIsLoadImage;
	bool m_bIsVertically;
	int  m_iChannels;
	int  m_iWidth;
	int  m_iHeight;
	int  m_iScanline;
};