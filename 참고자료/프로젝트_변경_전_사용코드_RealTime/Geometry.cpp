#include "Geometry.h"

Vector2f operator*(const Matrix2x2& m, const Vector2f& v)
{
	return Vector2f(
		m.e00 * v.x + m.e01 * v.y,
		m.e10 * v.x + m.e11 * v.y
	);
}

Vector3f operator*(const Matrix3x3& m, const Vector3f& v)
{
	return Vector3f(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z
	);
}

Vector4f operator*(const Matrix4x4& m, const Vector4f& v)
{
	return Vector4f(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z + m.e03 * v.w,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z + m.e13 * v.w,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z + m.e23 * v.w,
		m.e30 * v.x + m.e31 * v.y + m.e32 * v.z + m.e33 * v.w
	);
}