#pragma once

#include "Vector.h"

using RGBColor = Vector3f;

unsigned char GetRGBRed(const RGBColor& color);
unsigned char GetRGBGreen(const RGBColor& color);
unsigned char GetRGBBlue(const RGBColor& color);

RGBColor Interpolation(const RGBColor& color0, const RGBColor& color1, int term, int length);