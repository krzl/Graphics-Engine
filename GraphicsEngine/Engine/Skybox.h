#pragma once

#include <string>

#include "Texture.h"
#include "Primitives\Cube.h"
#include "Shaders\ShaderManager.h"

namespace Kz
{
	class RenderSystem;
	class TextureCreator;
	class Viewport;

	class Skybox
	{
		friend class EntityManager;

	private:

		RenderPerEntityInfo		m_entityInfo;
		Cube*					m_cube;
		TextureCubemap*			m_cubemap;

		Skybox(EntityManager& modelManager, TextureCreator& textureCreator,
			const std::string& skyboxFilepath, ShaderProgram& shader);

	public:

		~Skybox();

		void					SetViewport(Viewport& viewport);
		void					PreRender(RenderSystem& renderSystem);
		void					Render();
	};
}