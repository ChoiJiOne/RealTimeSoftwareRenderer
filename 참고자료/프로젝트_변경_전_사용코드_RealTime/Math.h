#pragma once

const float PI = 3.14159265358979f;

inline float Radian(float degree)
{
    return degree * PI / 180.0f;
}

inline float Degree(float radian)
{
    return radian * 180.0f / PI;
}

inline float Clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline float Interpolation(float x0, float x1, int term, int length)
{
    float t = static_cast<float>(term) / static_cast<float>(length);
    return x0 + (x1 - x0) * t;
}