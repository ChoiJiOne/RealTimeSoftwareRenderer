#pragma once

#include "Vector2.h"

/*
 * class : Vector3f
 * float 타입의 3차원 벡터를 표현하는 클래스이다.
 * +, -, *, /, 과 같은 기초 연산과 내적, 외적, 정규화, 벡터의 길이 계산 등을 지원한다.
 */
class Vector3f
{
public:
	Vector3f()                             : component{ 0.0f, 0.0f, 0.0f }, x(component[0]), y(component[1]), z(component[2]) {}
	Vector3f(float _x, float _y, float _z) : component{ _x, _y, _z },       x(component[0]), y(component[1]), z(component[2]) {}
	Vector3f(const Vector3f& v)            : component{ v.x, v.y, v.z },    x(component[0]), y(component[1]), z(component[2]) {}
	Vector3f(const Vector2f& v, float _z)  : component{ v.x, v.y, _z },     x(component[0]), y(component[1]), z(component[2]) {}
	~Vector3f() {}

	Vector3f& operator=(const Vector3f& v);
	Vector3f& operator+=(const Vector3f& v);
	Vector3f& operator-=(const Vector3f& v);
	Vector3f& operator*=(const Vector3f& v);
	Vector3f& operator*=(const float scale);
	Vector3f& operator/=(const float scale);

	float LengthSquared(void) const;
	float Length(void)        const;
	void  Normalize(void);

public:
	float& x;
	float& y;
	float& z;

private:
	float component[3];
};

Vector3f operator+(const Vector3f& u, const Vector3f& v);
Vector3f operator-(const Vector3f& u, const Vector3f& v);
Vector3f operator*(const Vector3f& u, const Vector3f& v);
Vector3f operator*(const float scale, const Vector3f& v);
Vector3f operator*(const Vector3f& u, const float scale);
Vector3f operator/(const Vector3f& u, const float scale);

float    Dot(const Vector3f& u, const Vector3f& v);
Vector3f Cross(const Vector3f& u, const Vector3f& v);
Vector3f UnitVector(const Vector3f& v);
float    MinComponent(const Vector3f& v);
float    MaxComponent(const Vector3f& v);
Vector3f MinVector(const Vector3f& u, const Vector3f& v);
Vector3f MaxVector(const Vector3f& u, const Vector3f& v);


/*
 * class : Vector3i
 * int 타입의 3차원 벡터를 표현하는 클래스이다.
 * +, -, *, 과 같은 기초 연산과 내적을 지원한다.
 * 이때, float 연산이 포함될만한 것들은 지원하지 않는다.
 * float 연산이 필요한 경우, Vector3f로 변환하여 사용해야 한다.
 */
class Vector3i
{
public:
	Vector3i()                          : component{ 0, 0, 0 },       x(component[0]), y(component[1]), z(component[2]) {}
	Vector3i(int _x, int _y, int _z)    : component{ _x, _y, _z },    x(component[0]), y(component[1]), z(component[2]) {}
	Vector3i(const Vector3i& v)         : component{ v.x, v.y, v.z }, x(component[0]), y(component[1]), z(component[2]) {}
	Vector3i(const Vector2i& v, int _z) : component{ v.x, v.y, _z },  x(component[0]), y(component[1]), z(component[2]) {}
	~Vector3i() {}

	Vector3i& operator=(const Vector3i& v);
	Vector3i& operator+=(const Vector3i& v);
	Vector3i& operator-=(const Vector3i& v);
	Vector3i& operator*=(const Vector3i& v);
	Vector3i& operator*=(const int scale);

public:
	int& x;
	int& y;
	int& z;

private:
	int component[3];
};

Vector3i operator+(const Vector3i& u, const Vector3i& v);
Vector3i operator-(const Vector3i& u, const Vector3i& v);
Vector3i operator*(const Vector3i& u, const Vector3i& v);
Vector3i operator*(const int scale, const Vector3i& v);
Vector3i operator*(const Vector3i& u, const int scale);

int      Dot(const Vector3i& u, const Vector3i& v);
Vector3i Cross(const Vector3i& u, const Vector3i& v);
int      MinComponent(const Vector3i& v);
int      MaxComponent(const Vector3i& v);
Vector3i MinVector(const Vector3i& u, const Vector3i& v);
Vector3i MaxVector(const Vector3i& u, const Vector3i& v);