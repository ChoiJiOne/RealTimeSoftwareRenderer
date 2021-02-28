#include <cmath>
#include <cassert>
#include <algorithm>
#include <limits>
#include "Vector3.h"

Vector3f& Vector3f::operator=(const Vector3f& v)
{
	if (this == &v) return *this;

	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vector3f& Vector3f::operator+=(const Vector3f& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3f& Vector3f::operator-=(const Vector3f& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3f& Vector3f::operator*=(const Vector3f& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

Vector3f& Vector3f::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}

Vector3f& Vector3f::operator/=(const float scale)
{
	assert(abs(scale - std::numeric_limits<float>::epsilon()) > 0.0f);

	x /= scale;
	y /= scale;
	z /= scale;

	return *this;
}

float Vector3f::LengthSquared(void) const
{
	return x * x + y * y + z * z;
}

float Vector3f::Length(void)  const
{
	return sqrt(LengthSquared());
}

void  Vector3f::Normalize(void)
{
	*this /= Length();
}

Vector3f operator+(const Vector3f& u, const Vector3f& v)
{
	return Vector3f(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vector3f operator-(const Vector3f& u, const Vector3f& v)
{
	return Vector3f(u.x - v.x, u.y - v.y, u.z - v.z);
}

Vector3f operator*(const Vector3f& u, const Vector3f& v)
{
	return Vector3f(u.x * v.x, u.y * v.y, u.z * v.z);
}

Vector3f operator*(const float scale, const Vector3f& v)
{
	return Vector3f(scale * v.x, scale * v.y, scale * v.z);
}

Vector3f operator*(const Vector3f& u, const float scale)
{
	return scale * u;
}

Vector3f operator/(const Vector3f& u, const float scale)
{
	assert(abs(scale - std::numeric_limits<float>::epsilon()) > 0.0f);

	return Vector3f(u.x / scale, u.y / scale, u.z / scale);
}

float Dot(const Vector3f& u, const Vector3f& v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

Vector3f Cross(const Vector3f& u, const Vector3f& v)
{
	return Vector3f(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

Vector3f UnitVector(const Vector3f& v)
{
	float length = v.Length();
	assert(abs(length - std::numeric_limits<float>::epsilon()) > 0.0f);

	return v / length;
}

float MinComponent(const Vector3f& v)
{
	return std::min(v.x, std::min(v.y, v.z));
}

float MaxComponent(const Vector3f& v)
{
	return std::max(v.x, std::max(v.y, v.z));

}

Vector3f MinVector(const Vector3f& u, const Vector3f& v)
{
	return Vector3f(
		std::min(u.x, v.x),
		std::min(u.y, v.y),
		std::min(u.z, v.z)
	);
}

Vector3f MaxVector(const Vector3f& u, const Vector3f& v)
{
	return Vector3f(
		std::max(u.x, v.x),
		std::max(u.y, v.y),
		std::max(u.z, v.z)
	);
}

Vector3i& Vector3i::operator=(const Vector3i& v)
{
	if (this == &v) return *this;

	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vector3i& Vector3i::operator+=(const Vector3i& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3i& Vector3i::operator-=(const Vector3i& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3i& Vector3i::operator*=(const Vector3i& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

Vector3i& Vector3i::operator*=(const int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}

Vector3i operator+(const Vector3i& u, const Vector3i& v)
{
	return Vector3i(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vector3i operator-(const Vector3i& u, const Vector3i& v)
{
	return Vector3i(u.x - v.x, u.y - v.y, u.z - v.z);
}

Vector3i operator*(const Vector3i& u, const Vector3i& v)
{
	return Vector3i(u.x * v.x, u.y * v.y, u.z * v.z);
}

Vector3i operator*(const int scale, const Vector3i& v)
{
	return Vector3i(scale * v.x, scale * v.y, scale * v.z);
}

Vector3i operator*(const Vector3i& u, const int scale)
{
	return Vector3i(u.x * scale, u.y * scale, u.z * scale);
}

int Dot(const Vector3i& u, const Vector3i& v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

Vector3i Cross(const Vector3i& u, const Vector3i& v)
{
	return Vector3i(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

int MinComponent(const Vector3i& v)
{
	return std::min(v.x, std::min(v.y, v.z));
}

int MaxComponent(const Vector3i& v)
{
	return std::max(v.x, std::max(v.y, v.z));
}

Vector3i MinVector(const Vector3i& u, const Vector3i& v)
{
	return Vector3i(
		std::min(u.x, v.x),
		std::min(u.y, v.y),
		std::min(u.z, v.z)
	);
}

Vector3i MaxVector(const Vector3i& u, const Vector3i& v)
{
	return Vector3i(
		std::max(u.x, v.x),
		std::max(u.y, v.y),
		std::max(u.z, v.z)
	);
}