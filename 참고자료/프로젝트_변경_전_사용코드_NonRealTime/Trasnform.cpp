#include <cassert>
#include <limits>
#include <cmath>
#include "Math.h"
#include "Transform.h"

Matrix4x4 Translate(float x, float y, float z)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f,    x,
		0.0f, 1.0f, 0.0f,    y,
		0.0f, 0.0f, 1.0f,    z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Translate(const Vector3f& translate)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, translate.x,
		0.0f, 1.0f, 0.0f, translate.y,
		0.0f, 0.0f, 1.0f, translate.z,
		0.0f, 0.0f, 0.0f,        1.0f
	);
}

Matrix4x4 Scale(float x, float y, float z)
{
	return Matrix4x4(
		   x, 0.0f, 0.0f, 0.0f,
		0.0f,    y, 0.0f, 0.0f,
		0.0f, 0.0f,    z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Scale(const Vector3f& scale)
{
	return Matrix4x4(
		scale.x,    0.0f,    0.0f, 0.0f,
		   0.0f, scale.y,    0.0f, 0.0f,
		   0.0f,    0.0f, scale.z, 0.0f,
		   0.0f,    0.0f,    0.0f, 1.0f
	);
}

Matrix4x4 RotateX(float radian)
{
	float sin_t = sin(radian);
	float cos_t = cos(radian);

	return Matrix4x4(
		1.0f,  0.0f,   0.0f, 0.0f,
		0.0f, cos_t, -sin_t, 0.0f,
		0.0f, sin_t,  cos_t, 0.0f,
		0.0f,  0.0f,   0.0f, 1.0f
	);
}

Matrix4x4 RotateY(float radian)
{
	float sin_t = sin(radian);
	float cos_t = cos(radian);

	return Matrix4x4(
		 cos_t, 0.0f, sin_t, 0.0f,
		  0.0f, 1.0f,  0.0f, 0.0f,
		-sin_t, 0.0f, cos_t, 0.0f,
		  0.0f, 0.0f,  0.0f, 1.0f
	);
}

Matrix4x4 RotateZ(float radian)
{
	float sin_t = sin(radian);
	float cos_t = cos(radian);

	return Matrix4x4(
		cos_t, -sin_t, 0.0f, 0.0f,
		sin_t, cos_t,  0.0f, 0.0f,
		 0.0f,  0.0f,  1.0f, 0.0f,
		 0.0f,  0.0f,  0.0f, 1.0f
	);
}

Matrix4x4 Rotate(float radian, const Vector3f& axis)
{
	Vector3f normalAxis = UnitVector(axis);

	float x = normalAxis.x;
	float y = normalAxis.y;
	float z = normalAxis.z;
	float sin_t = sin(radian);
	float cos_t = cos(radian);

	return Matrix4x4(
		    cos_t + (1.0f - cos_t) * x * x, (1.0f - cos_t) * x * y - sin_t * z, (1.0f - cos_t) * x * z + sin_t * y, 0.0f,
		(1.0f - cos_t) * x * y + sin_t * z,     cos_t + (1.0f - cos_t) * y * y, (1.0f - cos_t) * y * z - sin_t * x, 0.0f,
		(1.0f - cos_t) * x * z - sin_t * y, (1.0f - cos_t) * y * z + sin_t * x,        cos_t + (1 - cos_t) * z * z, 0.0f,
		                              0.0f,                               0.0f,                               0.0f, 1.0f
	);
}

Matrix4x4 LookAt(const Vector3f& lookFrom, const Vector3f& lookTo, const Vector3f& up)
{
	Vector3f zaxis = UnitVector(lookFrom - lookTo);
	Vector3f xaxis = UnitVector(Cross(up, zaxis));
	Vector3f yaxis = Cross(zaxis, xaxis);

	return Matrix4x4(
		xaxis.x, xaxis.y, xaxis.z, -Dot(xaxis, lookFrom),
		yaxis.x, yaxis.y, yaxis.z, -Dot(yaxis, lookFrom),
		zaxis.x, zaxis.y, zaxis.z, -Dot(zaxis, lookFrom),
	       0.0f,    0.0f,    0.0f,                  1.0f
	);
}

Matrix4x4 Perspective(float angle, float aspect, float nearZ, float farZ)
{
	assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
	float fov = tan(angle / 2.0f);

	return Matrix4x4( 
		1.0f / (fov * aspect),       0.0f,                             0.0f,                                   0.0f,
		                 0.0f, 1.0f / fov,                             0.0f,                                   0.0f,
		                 0.0f,       0.0f, -(farZ + nearZ) / (farZ - nearZ), -2.0f * farZ * nearZ / (farZ - nearZ),
		                 0.0f,       0.0f,                            -1.0f,                                   0.0f
	);
}

Matrix4x4 Viewport(int x, int y, int width, int height)
{
	float left    = static_cast<float>(x);
	float bottom  = static_cast<float>(y);
	float widthf  = static_cast<float>(width);
	float heightf = static_cast<float>(height);

	return Matrix4x4(
		widthf / 2.0f,           0.0f, 0.0f,    left + widthf / 2.0f,
		         0.0f, heightf / 2.0f, 0.0f, bottom + heightf / 2.0f,
		         0.0f,           0.0f, 1.0f,                    0.0f,
	      	     0.0f,           0.0f, 0.0f,                    1.0f
	);
}