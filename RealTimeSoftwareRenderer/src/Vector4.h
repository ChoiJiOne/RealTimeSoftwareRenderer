#pragma once

#include "Vector3.h"


/*
 * class : Vector4f
 * float 타입의 4차원 벡터를 표현하는 클래스이다.
 * +, -, *, /, 과 같은 기초 연산과 내적, 외적, 정규화, 벡터의 길이 계산 등을 지원한다.
 */
class Vector4f
{
public:
	Vector4f()                                       : component{ 0.0f, 0.0f, 0.0f, 0.0f }, x(component[0]), y(component[1]), z(component[2]), w(component[3]) {}
	Vector4f(float _x, float _y, float _z, float _w) : component{ _x, _y, _z, _w },         x(component[0]), y(component[1]), z(component[2]), w(component[3]) {}
	Vector4f(const Vector4f& v)                      : component{ v.x, v.y, v.z, v.w },     x(component[0]), y(component[1]), z(component[2]), w(component[3]) {}
	Vector4f(const Vector3f& v, float _w)            : component{ v.x, v.y, v.z, _w },      x(component[0]), y(component[1]), z(component[2]), w(component[3]) {}
	Vector4f(const Vector2f& v, float _z, float _w)  : component{ v.x, v.y, _z, _w },       x(component[0]), y(component[1]), z(component[2]), w(component[3]) {}
	~Vector4f() {}

	Vector4f& operator=(const Vector4f& v);
	Vector4f& operator+=(const Vector4f& v);
	Vector4f& operator-=(const Vector4f& v);
	Vector4f& operator*=(const Vector4f& v);
	Vector4f& operator*=(const float scale);
	Vector4f& operator/=(const float scale);

	float LengthSquared(void) const;
	float Length(void)        const;
	void  Normalize(void);

public:
	float& x;
	float& y;
	float& z;
	float& w;

private:
	float component[4];
};

Vector4f operator+(const Vector4f& u, const Vector4f& v);
Vector4f operator-(const Vector4f& u, const Vector4f& v);
Vector4f operator*(const Vector4f& u, const Vector4f& v);
Vector4f operator*(const float scale, const Vector4f& v);
Vector4f operator*(const Vector4f& u, const float scale);
Vector4f operator/(const Vector4f& u, const float scale);

float    Dot(const Vector4f& u, const Vector4f& v);
Vector4f UnitVector(const Vector4f& v);
float    MinComponent(const Vector4f& v);
float    MaxComponent(const Vector4f& v);
Vector4f MinVector(const Vector4f& u, const Vector4f& v);
Vector4f MaxVector(const Vector4f& u, const Vector4f& v);