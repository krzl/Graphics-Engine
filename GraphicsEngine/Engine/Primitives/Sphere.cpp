#include "stdafx.h"
#include "Sphere.h"
#include "RenderSystem.h"

namespace Kz
{
	Sphere::Sphere(Model& model, float x, float y, float z, GraphicsDevice& gd, Material& material) :
		Primitive(model, gd, material)
	{
		m_entityInfo.scaleVector = Vector3f(x, y, z);
	}

	Sphere::Sphere(Model& model, float x, float y, float z, GraphicsDevice& gd, ShaderProgram& shader) :
		Primitive(model, gd, shader)
	{
		m_entityInfo.scaleVector = Vector3f(x, y, z);
	}
}