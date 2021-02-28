#pragma once

#include "Vector.h"
#include "Matrix.h"

Matrix4x4 Translate(float x, float y, float z);
Matrix4x4 Translate(const Vector3f& translate);

Matrix4x4 Scale(float x, float y, float z);
Matrix4x4 Scale(const Vector3f& scale);

Matrix4x4 RotateX(float radian);
Matrix4x4 RotateY(float radian);
Matrix4x4 RotateZ(float radian);
Matrix4x4 Rotate(float radian, const Vector3f& axis);

Matrix4x4 LookAt(const Vector3f& lookFrom, const Vector3f& lookAt, const Vector3f& up);
Matrix4x4 Perspective(float angle, float aspect, float nearZ, float farZ);
Matrix4x4 Viewport(int x, int y, int width, int height);
