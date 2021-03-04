#pragma once

#include <algorithm>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

inline void Swap(Vector2f& u, Vector2f& v)
{
	std::swap(u.x, v.x);
	std::swap(u.y, v.y);
}

inline void Swap(Vector2i& u, Vector2i& v)
{
	std::swap(u.x, v.x);
	std::swap(u.y, v.y);
}

inline void Swap(Vector3f& u, Vector3f& v)
{
	std::swap(u.x, v.x);
	std::swap(u.y, v.y);
	std::swap(u.z, v.z);
}

inline void Swap(Vector3i& u, Vector3i& v)
{
	std::swap(u.x, v.x);
	std::swap(u.y, v.y);
	std::swap(u.z, v.z);
}

inline void Swap(Vector4f& u, Vector4f& v)
{
	std::swap(u.x, v.x);
	std::swap(u.y, v.y);
	std::swap(u.z, v.z);
	std::swap(u.w, v.w);
}