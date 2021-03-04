#include <cmath>
#include <cassert>
#include <algorithm>
#include <limits>
#include "Vector2.h"

Vector2f& Vector2f::operator=(const Vector2f& v)
{
	if (this == &v) return *this;

	x = v.x;
	y = v.y;

	return *this;
}

Vector2f& Vector2f::operator+=(const Vector2f& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Vector2f& Vector2f::operator*=(const Vector2f& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

Vector2f& Vector2f::operator*=(const float scale)
{
	x *= scale;
	y *= scale;

	return *this;
}

Vector2f& Vector2f::operator/=(const float scale)
{
	assert(abs(scale - std::numeric_limits<float>::epsilon()) > 0.0f);

	x /= scale;
	y /= scale;

	return *this;
}

float Vector2f::LengthSquared(void) const 
{ 
	return x * x + y * y; 
}

float Vector2f::Length(void) const 
{ 
	return sqrt(LengthSquared()); 
}

void Vector2f::Normalize(void) 
{ 
	*this /= Length();
}

Vector2f operator+(const Vector2f& u, const Vector2f& v)
{
	return Vector2f(u.x + v.x, u.y + v.y);
}

Vector2f operator-(const Vector2f& u, const Vector2f& v)
{
	return Vector2f(u.x - v.x, u.y - v.y);
}

Vector2f operator*(const Vector2f& u, const Vector2f& v)
{
	return Vector2f(u.x * v.x, u.y * v.y);
}

Vector2f operator*(const float scale, const Vector2f& v)
{
	return Vector2f(scale * v.x, scale * v.y);
}

Vector2f operator*(const Vector2f& u, const float scale)
{
	return Vector2f(u.x * scale, u.y * scale);
}

Vector2f operator/(const Vector2f& u, const float scale)
{
	assert(abs(scale - std::numeric_limits<float>::epsilon()) > 0.0f);

	return Vector2f(u.x / scale, u.y / scale);
}

float Dot(const Vector2f& u, const Vector2f& v)
{
	return (u.x * v.x + u.y * v.y);
}

Vector2f UnitVector(const Vector2f& v)
{
	float length = v.Length();
	assert(abs(length - std::numeric_limits<float>::epsilon()) > 0.0f);

	return v / length;
}

float MinComponent(const Vector2f& v)
{
	return std::min(v.x, v.y);
}

float MaxComponent(const Vector2f& v)
{
	return std::max(v.x, v.y);
}

Vector2f MinVector(const Vector2f& u, const Vector2f& v)
{
	return Vector2f(
		std::min(u.x, v.x),
		std::min(u.y, v.y)
	);
}

Vector2f MaxVector(const Vector2f& u, const Vector2f& v)
{
	return Vector2f(
		std::max(u.x, v.x),
		std::max(u.y, v.y)
	);
}

Vector2i& Vector2i::operator=(const Vector2i& v)
{
	if (this == &v) return *this;

	x = v.x;
	y = v.y;

	return *this;
}

Vector2i& Vector2i::operator+=(const Vector2i& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2i& Vector2i::operator-=(const Vector2i& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Vector2i& Vector2i::operator*=(const Vector2i& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

Vector2i& Vector2i::operator*=(const int scale)
{
	x *= scale;
	y *= scale;

	return *this;
}

Vector2i operator+(const Vector2i& u, const Vector2i& v)
{
	return Vector2i(u.x + v.x, u.y + v.y);
}

Vector2i operator-(const Vector2i& u, const Vector2i& v)
{
	return Vector2i(u.x - v.x, u.y - v.y);
}

Vector2i operator*(const Vector2i& u, const Vector2i& v)
{
	return Vector2i(u.x * v.x, u.y * v.y);
}

Vector2i operator*(const int scale, const Vector2i& v)
{
	return Vector2i(scale * v.x, scale * v.y);
}

Vector2i operator*(const Vector2i& u, const int scale)
{
	return scale * u;
}

int Dot(const Vector2i& u, const Vector2i& v)
{
	return (u.x * v.x + u.y * v.y);
}

int MinComponent(const Vector2i& v)
{
	return std::min(v.x, v.y);
}

int MaxComponent(const Vector2i& v)
{
	return std::max(v.x, v.y);
}

Vector2i MinVector(const Vector2i& u, const Vector2i& v)
{
	return Vector2i(
		std::min(u.x, v.x),
		std::min(u.y, v.y)
	);
}

Vector2i MaxVector(const Vector2i& u, const Vector2i& v)
{
	return Vector2i(
		std::max(u.x, v.x),
		std::max(u.y, v.y)
	);
}