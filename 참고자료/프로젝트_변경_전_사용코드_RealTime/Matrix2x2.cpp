#include <cassert>
#include <algorithm>
#include "Matrix2x2.h"

Matrix2x2& Matrix2x2::operator=(const Matrix2x2& m)
{
	if (this == &m) return *this;

	e00 = m.e00, e01 = m.e01;
	e10 = m.e10, e11 = m.e11;

	return *this;
}

Matrix2x2& Matrix2x2::operator+=(const Matrix2x2& m)
{
	e00 += m.e00, e01 += m.e01;
	e10 += m.e10, e11 += m.e11;

	return *this;
}

Matrix2x2& Matrix2x2::operator-=(const Matrix2x2& m)
{
	e00 -= m.e00, e01 -= m.e01;
	e10 -= m.e10, e11 -= m.e11;

	return *this;
}

Matrix2x2& Matrix2x2::operator*=(const Matrix2x2& m)
{
	Matrix2x2 temp;

	temp.e00 = e00 * m.e00 + e01 * m.e10;
	temp.e01 = e00 * m.e01 + e01 * m.e11;
	temp.e10 = e10 * m.e00 + e11 * m.e10;
	temp.e11 = e10 * m.e01 + e11 * m.e11;

	*this = temp;

	return *this;
}

Matrix2x2& Matrix2x2::operator*=(const float scale)
{
	e00 *= scale, e01 *= scale;
	e10 *= scale, e11 *= scale;

	return *this;
}

Matrix2x2 operator+(const Matrix2x2& m, const Matrix2x2& n)
{
	return Matrix2x2(
		m.e00 + n.e00, m.e01 + n.e01,
		m.e10 + n.e10, m.e11 + n.e11
	);
}

Matrix2x2 operator-(const Matrix2x2& m, const Matrix2x2& n)
{
	return Matrix2x2(
		m.e00 - n.e00, m.e01 - n.e01,
		m.e10 - n.e10, m.e11 - n.e11
	);
}

Matrix2x2 operator*(const Matrix2x2& m, const Matrix2x2& n)
{
	Matrix2x2 temp;

	temp.e00 = m.e00 * n.e00 + m.e01 * n.e10;
	temp.e01 = m.e00 * n.e01 + m.e01 * n.e11;
	temp.e10 = m.e10 * n.e00 + m.e11 * n.e10;
	temp.e11 = m.e10 * n.e01 + m.e11 * n.e11;

	return temp;
}

Matrix2x2 operator*(const float scale, const Matrix2x2& n)
{
	return Matrix2x2(
		scale * n.e00, scale * n.e01,
		scale * n.e10, scale * n.e11
	);
}

Matrix2x2 operator*(const Matrix2x2& m, const float scale)
{
	return scale * m;
}

void Matrix2x2::Identity(void)
{
	e00 = 1.0f, e01 = 0.0f;
	e10 = 0.0f, e11 = 1.0f;
}

void Matrix2x2::Transpose(void)
{
	std::swap(e01, e10);
}

void Matrix2x2::Inverse(void)
{
	float D = Determinant();

	assert(abs(D - std::numeric_limits<float>::epsilon()) > 0.0f);

	std::swap(e00, e11);

	e00 /= D, e01 /= -D;
	e10 /= -D, e11 /= D;
}