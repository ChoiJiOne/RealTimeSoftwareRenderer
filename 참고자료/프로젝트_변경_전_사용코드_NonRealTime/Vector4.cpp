#include <cmath>
#include <cassert>
#include <algorithm>
#include <limits>
#include "Vector4.h"

Vector4f& Vector4f::operator=(const Vector4f& v)
{
	if (this == &v) return *this;

	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;

	return *this;
}

Vector4f& Vector4f::operator+=(const Vector4f& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

Vector4f& Vector4f::operator-=(const Vector4f& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

Vector4f& Vector4f::operator*=(const Vector4f& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

Vector4f& Vector4f::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	return *this;
}

Vector4f& Vector4f::operator/=(const float scale)
{
	assert(abs(scale - std::numeric_limits<float>::epsilon()) > 0.0f);

	x /= scale;
	y /= scale;
	z /= scale;
	w /= scale;

	return *this;
}

float Vector4f::LengthSquared(void) const 
{ 
	return x * x + y * y + z * z + w * w; 
}

float Vector4f::Length(void)  const 
{ 
	return sqrt(LengthSquared()); 
}

void  Vector4f::Normalize(void) 
{ 
	*this /= Length(); 
}

Vector4f operator+(const Vector4f& u, const Vector4f& v)
{
	return Vector4f(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

Vector4f operator-(const Vector4f& u, const Vector4f& v)
{
	return Vector4f(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

Vector4f operator*(const Vector4f& u, const Vector4f& v)
{
	return Vector4f(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
}

Vector4f operator*(const float scale, const Vector4f& v)
{
	return Vector4f(scale * v.x, scale * v.y, scale * v.z, scale * v.w);
}

Vector4f operator*(const Vector4f& u, const float scale)
{
	return scale * u;
}

Vector4f operator/(const Vector4f& u, const float scale)
{
	assert(abs(scale - std::numeric_limits<float>::epsilon()) > 0.0f);

	return Vector4f(u.x / scale, u.y / scale, u.z / scale, u.w / scale);
}

float Dot(const Vector4f& u, const Vector4f& v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w);
}

Vector4f UnitVector(const Vector4f& v)
{
	float length = v.Length();
	assert(abs(length - std::numeric_limits<float>::epsilon()) > 0.0f);

	return v / length;
}

float MinComponent(const Vector4f& v)
{
	return std::min(v.x, std::min(v.y, std::min(v.z, v.w)));
}

float MaxComponent(const Vector4f& v)
{
	return std::max(v.x, std::max(v.y, std::max(v.z, v.w)));

}

Vector4f MinVector(const Vector4f& u, const Vector4f& v)
{
	return Vector4f(
		std::min(u.x, v.x),
		std::min(u.y, v.y),
		std::min(u.z, v.z),
		std::min(u.w, v.w)
	);
}

Vector4f MaxVector(const Vector4f& u, const Vector4f& v)
{
	return Vector4f(
		std::max(u.x, v.x),
		std::max(u.y, v.y),
		std::max(u.z, v.z),
		std::max(u.w, v.w)
	);
}