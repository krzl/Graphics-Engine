#pragma once

#include <map>

namespace Kz
{
	class Texture1D;
	class Texture2D;
	class TextureCubemap;
	class GraphicsDevice;

	class TextureCreator
	{
		friend class SceneManager;

	private:

		GraphicsDevice&							m_gd;

		std::map<std::string, Texture1D*>		m_textures1D;
		std::map<std::string, Texture2D*>		m_textures2D;
		std::map<std::string, TextureCubemap*>	m_texturesCubemap;

		TextureCreator(GraphicsDevice& gd);

	public:

		Texture2D*		CreateTexture2D(float r, float g, float b);
		Texture2D*		CreateTexture2D(const std::string& textureFilepath);
		TextureCubemap*	CreateTextureCubemap(const std::string& textureFilepath);

		~TextureCreator();
	};
}