#pragma once

/*
 * class : Vector2f
 * float 타입의 2차원 벡터를 표현하는 클래스이다.
 * +, -, *, /, 과 같은 기초 연산과 내적, 정규화, 벡터의 길이 계산 등을 지원한다.
 */
class Vector2f
{
public:
	Vector2f()                   : component{ 0.0f, 0.0f }, x(component[0]), y(component[1]) {}
	Vector2f(float _x, float _y) : component{ _x, _y },     x(component[0]), y(component[1]) {}
	Vector2f(const Vector2f& v)  : component{ v.x, v.y },   x(component[0]), y(component[1]) {}
	~Vector2f() {}

	Vector2f& operator=(const Vector2f& v);
	Vector2f& operator+=(const Vector2f& v);
	Vector2f& operator-=(const Vector2f& v);
	Vector2f& operator*=(const Vector2f& v);
	Vector2f& operator*=(const float scale);
	Vector2f& operator/=(const float scale);

	float LengthSquared(void) const;
	float Length(void)        const;
	void  Normalize(void);

public:
	float& x;
	float& y;

private:
	float component[2];
};

Vector2f operator+(const Vector2f& u, const Vector2f& v);
Vector2f operator-(const Vector2f& u, const Vector2f& v);
Vector2f operator*(const Vector2f& u, const Vector2f& v);
Vector2f operator*(const float scale, const Vector2f& v);
Vector2f operator*(const Vector2f& u, const float scale);
Vector2f operator/(const Vector2f& u, const float scale);

float    Dot(const Vector2f& u, const Vector2f& v);
Vector2f UnitVector(const Vector2f& v);
float    MinComponent(const Vector2f& v);
float    MaxComponent(const Vector2f& v);
Vector2f MinVector(const Vector2f& u, const Vector2f& v);
Vector2f MaxVector(const Vector2f& u, const Vector2f& v);


/*
 * class : Vector2i
 * int 타입의 2차원 벡터를 표현하는 클래스이다.
 * +, -, *, 과 같은 기초 연산과 내적을 지원한다.
 * 이때, float 연산이 포함될만한 것들은 지원하지 않는다.
 * float 연산이 필요한 경우, Vector2f로 변환하여 사용해야 한다.
 */
class Vector2i
{
public:
	Vector2i() : component{ 0, 0 }, x(component[0]), y(component[1]) {}
	Vector2i(int _x, int _y) : component{ _x, _y }, x(component[0]), y(component[1]) {}
	Vector2i(const Vector2i& v) : component{ v.x, v.y }, x(component[0]), y(component[1]) {}
	~Vector2i() {}

	Vector2i& operator=(const Vector2i& v);
	Vector2i& operator+=(const Vector2i& v);
	Vector2i& operator-=(const Vector2i& v);
	Vector2i& operator*=(const Vector2i& v);
	Vector2i& operator*=(const int scale);

public:
	int& x;
	int& y;

private:
	int component[2];
};

Vector2i operator+(const Vector2i& u, const Vector2i& v);
Vector2i operator-(const Vector2i& u, const Vector2i& v);
Vector2i operator*(const Vector2i& u, const Vector2i& v);
Vector2i operator*(const int scale, const Vector2i& v);
Vector2i operator*(const Vector2i& u, const int scale);

int Dot(const Vector2i& u, const Vector2i& v);
int MinComponent(const Vector2i& v);
int MaxComponent(const Vector2i& v);
Vector2i MinVector(const Vector2i& u, const Vector2i& v);
Vector2i MaxVector(const Vector2i& u, const Vector2i& v);
