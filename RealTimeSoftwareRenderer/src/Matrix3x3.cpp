#include <cassert>
#include <algorithm>
#include "Matrix3x3.h"

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& m)
{
	if (this == &m) return *this;

	e00 = m.e00, e01 = m.e01, e02 = m.e02;
	e10 = m.e10, e11 = m.e11, e12 = m.e12;
	e20 = m.e20, e21 = m.e21, e22 = m.e22;

	return *this;
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& m)
{
	e00 += m.e00, e01 += m.e01, e02 += m.e02;
	e10 += m.e10, e11 += m.e11, e12 += m.e12;
	e20 += m.e20, e21 += m.e21, e22 += m.e22;

	return *this;
}

Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& m)
{
	e00 -= m.e00, e01 -= m.e01, e02 -= m.e02;
	e10 -= m.e10, e11 -= m.e11, e12 -= m.e12;
	e20 -= m.e20, e21 -= m.e21, e22 -= m.e22;

	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& m)
{
	Matrix3x3 temp;

	temp.e00 = e00 * m.e00 + e01 * m.e10 + e02 * m.e20;
	temp.e01 = e00 * m.e01 + e01 * m.e11 + e02 * m.e21;
	temp.e02 = e00 * m.e02 + e01 * m.e12 + e02 * m.e22;


	temp.e10 = e10 * m.e00 + e11 * m.e10 + e12 * m.e20;
	temp.e11 = e10 * m.e01 + e11 * m.e11 + e12 * m.e21;
	temp.e12 = e10 * m.e02 + e11 * m.e12 + e12 * m.e22;

	temp.e20 = e20 * m.e00 + e21 * m.e10 + e22 * m.e20;
	temp.e21 = e20 * m.e01 + e21 * m.e11 + e22 * m.e21;
	temp.e22 = e20 * m.e02 + e21 * m.e12 + e22 * m.e22;

	*this = temp;

	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const float scale)
{
	e00 *= scale, e01 *= scale, e02 *= scale;
	e10 *= scale, e11 *= scale, e12 *= scale;
	e20 *= scale, e21 *= scale, e22 *= scale;

	return *this;
}

Matrix3x3 operator+(const Matrix3x3& m, const Matrix3x3& n)
{
	return Matrix3x3(
		m.e00 + n.e00, m.e01 + n.e01, m.e02 + n.e02,
		m.e10 + n.e10, m.e11 + n.e11, m.e12 + n.e12,
		m.e20 + n.e20, m.e21 + n.e21, m.e22 + n.e22
	);
}

Matrix3x3 operator-(const Matrix3x3& m, const Matrix3x3& n)
{
	return Matrix3x3(
		m.e00 - n.e00, m.e01 - n.e01, m.e02 - n.e02,
		m.e10 - n.e10, m.e11 - n.e11, m.e12 - n.e12,
		m.e20 - n.e20, m.e21 - n.e21, m.e22 - n.e22
	);
}

Matrix3x3 operator*(const Matrix3x3& m, const Matrix3x3& n)
{
	Matrix3x3 temp;

	temp.e00 = m.e00 * n.e00 + m.e01 * n.e10 + m.e02 * n.e20;
	temp.e01 = m.e00 * n.e01 + m.e01 * n.e11 + m.e02 * n.e21;
	temp.e02 = m.e00 * n.e02 + m.e01 * n.e12 + m.e02 * n.e22;


	temp.e10 = m.e10 * n.e00 + m.e11 * n.e10 + m.e12 * n.e20;
	temp.e11 = m.e10 * n.e01 + m.e11 * n.e11 + m.e12 * n.e21;
	temp.e12 = m.e10 * n.e02 + m.e11 * n.e12 + m.e12 * n.e22;

	temp.e20 = m.e20 * n.e00 + m.e21 * n.e10 + m.e22 * n.e20;
	temp.e21 = m.e20 * n.e01 + m.e21 * n.e11 + m.e22 * n.e21;
	temp.e22 = m.e20 * n.e02 + m.e21 * n.e12 + m.e22 * n.e22;

	return temp;
}

Matrix3x3 operator*(const float scale, const Matrix3x3& n)
{
	return Matrix3x3(
		scale * n.e00, scale * n.e01, scale * n.e02,
		scale * n.e10, scale * n.e11, scale * n.e12,
		scale * n.e20, scale * n.e21, scale * n.e22
	);
}

Matrix3x3 operator*(const Matrix3x3& m, const float scale)
{
	return scale * m;
}

void Matrix3x3::Identity(void)
{
	e00 = 1.0f, e01 = 0.0f, e02 = 0.0f;
	e10 = 0.0f, e11 = 1.0f, e12 = 0.0f;
	e20 = 0.0f, e21 = 0.0f, e22 = 1.0f;
}

void Matrix3x3::Transpose(void)
{
	std::swap(e01, e10);
	std::swap(e02, e20);
	std::swap(e12, e21);
}

float Matrix3x3::Determinant(void) const
{
	float D0 = e11 * e22 - e12 * e21;
	float D1 = e10 * e22 - e12 * e20;
	float D2 = e10 * e21 - e11 * e20;

	return e00 * D0 - e01 * D1 + e02 * D2;
}

void Matrix3x3::Inverse(void)
{
	float D = Determinant();

	assert(abs(D - std::numeric_limits<float>::epsilon()) > 0.0f);

	Matrix3x3 temp;

	temp.e00 = (e11 * e22 - e12 * e21) / D;
	temp.e01 = -(e01 * e22 - e02 * e21) / D;
	temp.e02 = (e01 * e12 - e02 * e11) / D;

	temp.e10 = -(e10 * e22 - e12 * e20) / D;
	temp.e11 = (e00 * e22 - e02 * e20) / D;
	temp.e12 = -(e00 * e12 - e02 * e10) / D;

	temp.e20 = (e10 * e21 - e11 * e20) / D;
	temp.e21 = -(e00 * e21 - e01 * e20) / D;
	temp.e22 = (e00 * e11 - e01 * e10) / D;

	*this = temp;
}