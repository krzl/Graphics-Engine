#pragma once

#include "Primitive.h"

namespace Kz
{
	class Cube : public Primitive
	{
		friend class DirectionalLight;
		friend class SpotLight;
		friend class EntityManager;

	private:

		Cube(Model& cubeModel, float x, float y, float z, GraphicsDevice& gd, Material& material);
		Cube(Model& cubeModel, float x, float y, float z, GraphicsDevice& gd, ShaderProgram& shader);
	};
}