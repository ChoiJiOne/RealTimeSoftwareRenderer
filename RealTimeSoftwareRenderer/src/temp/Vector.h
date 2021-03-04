#pragma once

#include <cmath>

/*
 * struct : Vec2
 * 2차원 벡터를 나타내는 템플릿 구조체로, union 키워드를 사용하여 2차원 텍스쳐 좌표로 사용할 수 있게 확장하였다.
 */
template <typename T>
struct Vec2
{
	Vec2()                 : x(static_cast<T>(0)), y(static_cast<T>(0)) {}
	Vec2(T _x, T _y)       : x(_x), y(_y)                               {}
	Vec2(const Vec2<T>& v) : x(v.x), y(v.y)                             {}

	inline Vec2<T>& operator=(const Vec2<T>& v) { if (this != &v) x = v.x, y = v.y; return *this; }

	inline T     LengthSquared(void) const { return x * x + y * y; }
	inline float Length(void)        const { return std::sqrt(LengthSquared()); }

	union
	{
		struct { T x, y; };
		struct { T s, t; };
	};
};

template <typename T> inline Vec2<T> operator+(const Vec2<T>& u, const Vec2<T>& v) { return Vec2<T>(u.x + v.x, u.y + v.y); }
template <typename T> inline Vec2<T> operator-(const Vec2<T>& u, const Vec2<T>& v) { return Vec2<T>(u.x - v.x, u.y - v.y); }
template <typename T> inline Vec2<T> operator*(const Vec2<T>& u, const Vec2<T>& v) { return Vec2<T>(u.x * v.x, u.y * v.y); }
template <typename T> inline Vec2<T> operator*(const T scale,    const Vec2<T>& v) { return Vec2<T>(scale * v.x, scale * v.y); }
template <typename T> inline Vec2<T> operator*(const Vec2<T>& u, const T scale)    { return Vec2<T>(u.x * scale, u.y * scale); }
template <typename T> inline T       Dot(const Vec2<T>& u, const Vec2<T>& v)       { return u.x * v.x + u.y * v.y; }

/*
 * struct : Vec3
 * 3차원 벡터를 나타내는 템플릿 구조체로, union 키워드를 사용하여 3차원 텍스쳐좌표와 RGB 색으로 사용할 수 있도록 확장했다.
 */
template <typename T>
struct Vec3
{
	Vec3()                       : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {}
	Vec3(T _x, T _y, T _z)       : x(_x), y(_y), z(_z)                                              {}
	Vec3(const Vec2<T>& v, T _z) : x(v.x), y(v.y), z(_z)                                            {}
	Vec3(const Vec3<T>& v)       : x(v.x), y(v.y), z(v.z)                                           {}

	inline Vec3<T>& operator=(const Vec3<T>& v) { if (this != &v) x = v.x, y = v.y, z = v.z; return *this; }

	inline T     LengthSquared(void) const { return x * x + y * y + z * z; }
	inline float Length(void)        const { return std::sqrt(LengthSquared()); }

	union
	{
		struct { T x, y, z; };
		struct { T s, t, p; };
		struct { T r, g, b; };
	};
};

template <typename T> inline Vec3<T> operator+(const Vec3<T>& u, const Vec3<T>& v) { return Vec3<T>(u.x + v.x, u.y + v.y, u.z + v.z); }
template <typename T> inline Vec3<T> operator-(const Vec3<T>& u, const Vec3<T>& v) { return Vec3<T>(u.x - v.x, u.y - v.y, u.z - v.z); }
template <typename T> inline Vec3<T> operator*(const Vec3<T>& u, const Vec3<T>& v) { return Vec3<T>(u.x * v.x, u.y * v.y, u.z * v.z); }
template <typename T> inline Vec3<T> operator*(const T scale,    const Vec3<T>& v) { return Vec3<T>(scale * v.x, scale * v.y, scale * v.z); }
template <typename T> inline Vec3<T> operator*(const Vec3<T>& u, const T scale)    { return Vec3<T>(u.x * scale, u.y * scale, u.z * scale); }
template <typename T> inline T       Dot(const Vec3<T>& u, const Vec3<T>& v)       { return u.x * v.x + u.y * v.y + u.z * v.z; }
template <typename T> inline Vec3<T> Cross(const Vec3<T>& u, const Vec3<T>& v)     { return Vec3<T>(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }

/*
 * struct : Vec4
 * 4차원 벡터를 나타내는 템플릿 구조체로, union 키워드를 사용하여 4차원 텍스쳐좌표와 RGBA 색으로 사용할 수 있도록 확장했다.
 */
template <typename T>
struct Vec4
{
	Vec4()                             : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0)) {}
	Vec4(T _x, T _y, T _z, T _w)       : x(_x), y(_y), z(_z), w(_w)                                                             {}
	Vec4(const Vec2<T>& v, T _z, T _w) : x(v.x), y(v.y), z(_z), w(_w)                                                           {}
	Vec4(const Vec3<T>& v, T _w)       : x(v.x), y(v.y), z(v.z), w(_w)                                                          {}
	Vec4(const Vec4<T>& v)             : x(v.x), y(v.y), z(v.z), w(v.w)                                                         {}

	inline Vec4<T>& operator=(const Vec4<T>& v) { if (this != &v) x = v.x, y = v.y, z = v.z, w = v.w; return *this; }

	inline T     LengthSquared(void) const { return x * x + y * y + z * z + w * w; }
	inline float Length(void)        const { return std::sqrt(LengthSquared()); }

	union
	{
		struct { T x, y, z, w; };
		struct { T s, t, p, q; };
		struct { T r, g, b, a; };
	};
};

template <typename T> inline Vec4<T> operator+(const Vec4<T>& u, const Vec4<T>& v) { return Vec4<T>(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w); }
template <typename T> inline Vec4<T> operator-(const Vec4<T>& u, const Vec4<T>& v) { return Vec4<T>(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); }
template <typename T> inline Vec4<T> operator*(const Vec4<T>& u, const Vec4<T>& v) { return Vec4<T>(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w); }
template <typename T> inline Vec4<T> operator*(const T scale,    const Vec4<T>& v) { return Vec4<T>(scale * v.x, scale * v.y, scale * v.z, scale * v.w); }
template <typename T> inline Vec4<T> operator*(const Vec4<T>& u, const T scale)    { return Vec4<T>(u.x * scale, u.y * scale, u.z * scale, u.w * scale); }
template <typename T> inline T       Dot(const Vec4<T>& u, const Vec4<T>& v)       { return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w; }

/*
 * Define type
 */
using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using RGB24 = Vec3<float>;
using Vec4i = Vec4<int>;
using Vec4f = Vec4<float>;
using RGB32 = Vec4<float>;