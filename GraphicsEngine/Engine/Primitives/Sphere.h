#pragma once

#include "Primitive.h"

namespace Kz
{
	class Texture2D;

	class Sphere : public Primitive
	{
		friend class PointLight;
		friend class EntityManager;

	private:

		Sphere(Model& sphereModel, float x, float y, float z, GraphicsDevice& gd, Material& material);
		Sphere(Model& sphereModel, float x, float y, float z, GraphicsDevice& gd, ShaderProgram& shader);
	};
}