#include "Math.h"
#include "FlatShader.h"

Vector3f FlatShader::VertexShader(const Vector3f& v)
{
	Vector4f position(v, 1.0f);

	position = m_Projection * m_View * m_Model * position;
	position /= position.w;
	position = m_Viewport * position;

	return Vector3f(position.x, position.y, position.z);
}

RGBColor FlatShader::FragmentShader(const Vector3f& n, const RGBColor& objectColor)
{
	float theta = Dot(m_LightDir, n);
	theta = std::abs(theta);

	return theta * objectColor;
}