#pragma once

class Matrix3x3
{
public:
	Matrix3x3()
		: e00(0.0f), e01(0.0f), e02(0.0f)
		, e10(0.0f), e11(0.0f), e12(0.0f)
		, e20(0.0f), e21(0.0f), e22(0.0f) {}

	Matrix3x3(float _e00, float _e01, float _e02,
		float _e10, float _e11, float _e12,
		float _e20, float _e21, float _e22)
		: e00(_e00), e01(_e01), e02(_e02)
		, e10(_e10), e11(_e11), e12(_e12)
		, e20(_e20), e21(_e21), e22(_e22) {}

	Matrix3x3(const Matrix3x3& m)
		: e00(m.e00), e01(m.e01), e02(m.e02)
		, e10(m.e10), e11(m.e11), e12(m.e12)
		, e20(m.e20), e21(m.e21), e22(m.e22) {}

	~Matrix3x3() {}

	Matrix3x3& operator=(const Matrix3x3& m);
	Matrix3x3& operator+=(const Matrix3x3& m);
	Matrix3x3& operator-=(const Matrix3x3& m);
	Matrix3x3& operator*=(const Matrix3x3& m);
	Matrix3x3& operator*=(const float scale);

	float Determinant(void) const;
	void  Identity(void);
	void  Transpose(void);
	void  Inverse(void);

public:
	float e00, e01, e02;
	float e10, e11, e12;
	float e20, e21, e22;
};

Matrix3x3 operator+(const Matrix3x3& m, const Matrix3x3& n);
Matrix3x3 operator-(const Matrix3x3& m, const Matrix3x3& n);
Matrix3x3 operator*(const Matrix3x3& m, const Matrix3x3& n);
Matrix3x3 operator*(const float scale, const Matrix3x3& n);
Matrix3x3 operator*(const Matrix3x3& m, const float scale);