#pragma once

class Matrix2x2
{
public:
	Matrix2x2()
		: e00(0.0f), e01(0.0f)
		, e10(0.0f), e11(0.0f) {}

	Matrix2x2(float _e00, float _e01,
		float _e10, float _e11)
		: e00(_e00), e01(_e01)
		, e10(_e10), e11(_e11) {}

	Matrix2x2(const Matrix2x2& m)
		: e00(m.e00), e01(m.e01)
		, e10(m.e10), e11(m.e11) {}

	~Matrix2x2() {}

	Matrix2x2& operator=(const Matrix2x2& m);
	Matrix2x2& operator+=(const Matrix2x2& m);
	Matrix2x2& operator-=(const Matrix2x2& m);
	Matrix2x2& operator*=(const Matrix2x2& m);
	Matrix2x2& operator*=(const float scale);

	float Determinant(void) const { return e00 * e11 - e01 * e10; }
	void  Identity(void);
	void  Transpose(void);
	void  Inverse(void);

public:
	float e00, e01;
	float e10, e11;
};

Matrix2x2 operator+(const Matrix2x2& m, const Matrix2x2& n);
Matrix2x2 operator-(const Matrix2x2& m, const Matrix2x2& n);
Matrix2x2 operator*(const Matrix2x2& m, const Matrix2x2& n);
Matrix2x2 operator*(const float scale, const Matrix2x2& n);
Matrix2x2 operator*(const Matrix2x2& m, const float scale);