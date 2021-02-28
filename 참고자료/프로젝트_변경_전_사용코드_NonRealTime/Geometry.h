#pragma once

#include "Vector.h"
#include "Matrix.h"

Vector2f operator*(const Matrix2x2& m, const Vector2f& v);
Vector3f operator*(const Matrix3x3& m, const Vector3f& v);
Vector4f operator*(const Matrix4x4& m, const Vector4f& v);