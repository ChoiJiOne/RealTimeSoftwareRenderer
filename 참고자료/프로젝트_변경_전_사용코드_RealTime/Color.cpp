#include "Math.h"
#include "Color.h"

unsigned char GetRGBRed(const RGBColor& color)
{
	float red = Clamp(color.x, 0.0f, 1.0f);

	return static_cast<unsigned char>(255.0f * red);
}

unsigned char GetRGBGreen(const RGBColor& color)
{
	float green = Clamp(color.y, 0.0f, 1.0f);

	return static_cast<unsigned char>(255.0f * green);
}

unsigned char GetRGBBlue(const RGBColor& color)
{
	float blue = Clamp(color.z, 0.0f, 1.0f);

	return static_cast<unsigned char>(255.0f * blue);
}

RGBColor Interpolation(const RGBColor& color0, const RGBColor& color1, int term, int length)
{
	float scale = static_cast<float>(term) / static_cast<float>(length);
	return color0 + (scale * (color1 - color0));
}