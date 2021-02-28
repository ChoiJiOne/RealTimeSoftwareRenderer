#pragma once

class Matrix4x4
{
public:
	Matrix4x4()
		: e00(0.0f), e01(0.0f), e02(0.0f), e03(0.0f)
		, e10(0.0f), e11(0.0f), e12(0.0f), e13(0.0f)
		, e20(0.0f), e21(0.0f), e22(0.0f), e23(0.0f)
		, e30(0.0f), e31(0.0f), e32(0.0f), e33(0.0f) {}

	Matrix4x4(float _e00, float _e01, float _e02, float _e03,
		float _e10, float _e11, float _e12, float _e13,
		float _e20, float _e21, float _e22, float _e23,
		float _e30, float _e31, float _e32, float _e33)
		: e00(_e00), e01(_e01), e02(_e02), e03(_e03)
		, e10(_e10), e11(_e11), e12(_e12), e13(_e13)
		, e20(_e20), e21(_e21), e22(_e22), e23(_e23)
		, e30(_e30), e31(_e31), e32(_e32), e33(_e33) {}

	Matrix4x4(const Matrix4x4& m)
		: e00(m.e00), e01(m.e01), e02(m.e02), e03(m.e03)
		, e10(m.e10), e11(m.e11), e12(m.e12), e13(m.e13)
		, e20(m.e20), e21(m.e21), e22(m.e22), e23(m.e23)
		, e30(m.e30), e31(m.e31), e32(m.e32), e33(m.e33) {}

	~Matrix4x4() {}

	Matrix4x4& operator=(const Matrix4x4& m);
	Matrix4x4& operator+=(const Matrix4x4& m);
	Matrix4x4& operator-=(const Matrix4x4& m);
	Matrix4x4& operator*=(const Matrix4x4& m);
	Matrix4x4& operator*=(const float scale);

	void  Identity(void);
	void  Transpose(void);

public:
	float e00, e01, e02, e03;
	float e10, e11, e12, e13;
	float e20, e21, e22, e23;
	float e30, e31, e32, e33;
};

Matrix4x4 operator+(const Matrix4x4& m, const Matrix4x4& n);
Matrix4x4 operator-(const Matrix4x4& m, const Matrix4x4& n);
Matrix4x4 operator*(const Matrix4x4& m, const Matrix4x4& n);
Matrix4x4 operator*(const float scale, const Matrix4x4& n);
Matrix4x4 operator*(const Matrix4x4& m, const float scale);