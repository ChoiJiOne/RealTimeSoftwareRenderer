#pragma once

/*
 * class : Vector2f
 * float Ÿ���� 2���� ���͸� ǥ���ϴ� Ŭ�����̴�.
 * +, -, *, /, �� ���� ���� ����� ����, ����ȭ, ������ ���� ��� ���� �����Ѵ�.
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
 * int Ÿ���� 2���� ���͸� ǥ���ϴ� Ŭ�����̴�.
 * +, -, *, �� ���� ���� ����� ������ �����Ѵ�.
 * �̶�, float ������ ���Եɸ��� �͵��� �������� �ʴ´�.
 * float ������ �ʿ��� ���, Vector2f�� ��ȯ�Ͽ� ����ؾ� �Ѵ�.
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
