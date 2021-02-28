#include <cassert>
#include <algorithm>
#include "Matrix4x4.h"

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& m)
{
	if (this == &m) return *this;

	e00 = m.e00, e01 = m.e01, e02 = m.e02, e03 = m.e03;
	e10 = m.e10, e11 = m.e11, e12 = m.e12, e13 = m.e13;
	e20 = m.e20, e21 = m.e21, e22 = m.e22, e23 = m.e23;
	e30 = m.e30, e31 = m.e31, e32 = m.e32, e33 = m.e33;

	return *this;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
{
	e00 += m.e00, e01 += m.e01, e02 += m.e02, e03 += m.e03;
	e10 += m.e10, e11 += m.e11, e12 += m.e12, e13 += m.e13;
	e20 += m.e20, e21 += m.e21, e22 += m.e22, e23 += m.e23;
	e30 += m.e30, e31 += m.e31, e32 += m.e32, e33 += m.e33;

	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& m)
{
	e00 -= m.e00, e01 -= m.e01, e02 -= m.e02, e03 -= m.e03;
	e10 -= m.e10, e11 -= m.e11, e12 -= m.e12, e13 -= m.e13;
	e20 -= m.e20, e21 -= m.e21, e22 -= m.e22, e23 -= m.e23;
	e30 -= m.e30, e31 -= m.e31, e32 -= m.e32, e33 -= m.e33;

	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
{
	Matrix4x4 temp;

	temp.e00 = e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30;
	temp.e01 = e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31;
	temp.e02 = e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32;
	temp.e03 = e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33;

	temp.e10 = e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30;
	temp.e11 = e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31;
	temp.e12 = e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32;
	temp.e13 = e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33;

	temp.e20 = e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30;
	temp.e21 = e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31;
	temp.e22 = e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32;
	temp.e23 = e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33;

	temp.e30 = e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30;
	temp.e31 = e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31;
	temp.e32 = e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32;
	temp.e33 = e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33;

	*this = temp;

	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const float scale)
{
	e00 *= scale, e01 *= scale, e02 *= scale, e03 *= scale;
	e10 *= scale, e11 *= scale, e12 *= scale, e13 *= scale;
	e20 *= scale, e21 *= scale, e22 *= scale, e23 *= scale;
	e30 *= scale, e31 *= scale, e32 *= scale, e33 *= scale;

	return *this;
}

Matrix4x4 operator+(const Matrix4x4& m, const Matrix4x4& n)
{
	return Matrix4x4(
		m.e00 + n.e00, m.e01 + n.e01, m.e02 + n.e02, m.e03 + n.e03,
		m.e10 + n.e10, m.e11 + n.e11, m.e12 + n.e12, m.e13 + n.e13,
		m.e20 + n.e20, m.e21 + n.e21, m.e22 + n.e22, m.e23 + n.e23,
		m.e30 + n.e30, m.e31 + n.e31, m.e32 + n.e32, m.e33 + n.e33
	);
}

Matrix4x4 operator-(const Matrix4x4& m, const Matrix4x4& n)
{
	return Matrix4x4(
		m.e00 - n.e00, m.e01 - n.e01, m.e02 - n.e02, m.e03 - n.e03,
		m.e10 - n.e10, m.e11 - n.e11, m.e12 - n.e12, m.e13 - n.e13,
		m.e20 - n.e20, m.e21 - n.e21, m.e22 - n.e22, m.e23 - n.e23,
		m.e30 - n.e30, m.e31 - n.e31, m.e32 - n.e32, m.e33 - n.e33
	);
}

Matrix4x4 operator*(const Matrix4x4& m, const Matrix4x4& n)
{
	Matrix4x4 temp;

	temp.e00 = m.e00 * n.e00 + m.e01 * n.e10 + m.e02 * n.e20 + m.e03 * n.e30;
	temp.e01 = m.e00 * n.e01 + m.e01 * n.e11 + m.e02 * n.e21 + m.e03 * n.e31;
	temp.e02 = m.e00 * n.e02 + m.e01 * n.e12 + m.e02 * n.e22 + m.e03 * n.e32;
	temp.e03 = m.e00 * n.e03 + m.e01 * n.e13 + m.e02 * n.e23 + m.e03 * n.e33;

	temp.e10 = m.e10 * n.e00 + m.e11 * n.e10 + m.e12 * n.e20 + m.e13 * n.e30;
	temp.e11 = m.e10 * n.e01 + m.e11 * n.e11 + m.e12 * n.e21 + m.e13 * n.e31;
	temp.e12 = m.e10 * n.e02 + m.e11 * n.e12 + m.e12 * n.e22 + m.e13 * n.e32;
	temp.e13 = m.e10 * n.e03 + m.e11 * n.e13 + m.e12 * n.e23 + m.e13 * n.e33;

	temp.e20 = m.e20 * n.e00 + m.e21 * n.e10 + m.e22 * n.e20 + m.e23 * n.e30;
	temp.e21 = m.e20 * n.e01 + m.e21 * n.e11 + m.e22 * n.e21 + m.e23 * n.e31;
	temp.e22 = m.e20 * n.e02 + m.e21 * n.e12 + m.e22 * n.e22 + m.e23 * n.e32;
	temp.e23 = m.e20 * n.e03 + m.e21 * n.e13 + m.e22 * n.e23 + m.e23 * n.e33;

	temp.e30 = m.e30 * n.e00 + m.e31 * n.e10 + m.e32 * n.e20 + m.e33 * n.e30;
	temp.e31 = m.e30 * n.e01 + m.e31 * n.e11 + m.e32 * n.e21 + m.e33 * n.e31;
	temp.e32 = m.e30 * n.e02 + m.e31 * n.e12 + m.e32 * n.e22 + m.e33 * n.e32;
	temp.e33 = m.e30 * n.e03 + m.e31 * n.e13 + m.e32 * n.e23 + m.e33 * n.e33;

	return temp;
}

Matrix4x4 operator*(const float scale, const Matrix4x4& n)
{
	return Matrix4x4(
		scale * n.e00, scale * n.e01, scale * n.e02, scale * n.e03,
		scale * n.e10, scale * n.e11, scale * n.e12, scale * n.e13,
		scale * n.e20, scale * n.e21, scale * n.e22, scale * n.e23,
		scale * n.e30, scale * n.e31, scale * n.e32, scale * n.e33
	);
}

Matrix4x4 operator*(const Matrix4x4& m, const float scale)
{
	return scale * m;
}

void Matrix4x4::Identity(void)
{
	e00 = 1.0f, e01 = 0.0f, e02 = 0.0f, e03 = 0.0f;
	e10 = 0.0f, e11 = 1.0f, e12 = 0.0f, e13 = 0.0f;
	e20 = 0.0f, e21 = 0.0f, e22 = 1.0f, e23 = 0.0f;
	e30 = 0.0f, e31 = 0.0f, e32 = 0.0f, e33 = 1.0f;
}

void Matrix4x4::Transpose(void)
{
	std::swap(e01, e10);
	std::swap(e02, e20);
	std::swap(e12, e21);
	std::swap(e03, e30);
	std::swap(e13, e31);
	std::swap(e23, e32);
}
