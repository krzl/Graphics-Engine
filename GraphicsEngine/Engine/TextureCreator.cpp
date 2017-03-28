#include "stdafx.h"
#include "TextureCreator.h"
#include "GraphicsDevice.h"

namespace Kz
{
	TextureCreator::TextureCreator(GraphicsDevice& gd) :
		m_gd(gd)
	{}

	Texture2D* TextureCreator::CreateTexture2D(float r, float g, float b)
	{
		std::string textureName = "__internal__" + std::to_string(r) + "_" +
			std::to_string(g) + "_" + std::to_string(b);

		std::map<std::string, Texture2D*>::iterator it = m_textures2D.find(textureName);

		if (it == m_textures2D.end())
		{
			Texture2D* texture = m_gd.Create1x1Texture(r, g, b);
			m_textures2D[textureName] = texture;

			return texture;
		}
		else
		{
			return it->second;
		}
	}

	Texture2D* TextureCreator::CreateTexture2D(const std::string& textureFilepath)
	{
		std::map<std::string, Texture2D*>::iterator it = m_textures2D.find(textureFilepath);

		if (it == m_textures2D.end())
		{
			TgaFile srcFile(textureFilepath);
			Texture2D* texture = m_gd.CreateTexture(srcFile.GetWidth(), srcFile.GetHeight(),
				&srcFile.GetImageData()[0], srcFile.GetBitsPerPixel() > 24 ? 
				FORMAT_RGBA : FORMAT_RGBX, FILTERING_MIN_LINEAR_MIPMAP_NEAREST, 
				WRAP_REPEAT, USAGE_IMMUTABLE, ACCESS_NO_ACCESS, true);

			m_textures2D[textureFilepath] = texture;
			return texture;
		}
		else{
			return it->second;
		}
	}

	TextureCubemap* TextureCreator::CreateTextureCubemap(const std::string& textureFilepath)
	{
		std::map<std::string, TextureCubemap*>::iterator it = 
			m_texturesCubemap.find(textureFilepath);

		if (it == m_texturesCubemap.end())
		{
			const std::string filenames[] = { "left", "right", "down", "up", "front", "back" };
			void* cubemapData[6];
			TgaFile* srcFiles[6];

			for (int i = 0; i < 6; i++)
			{
				srcFiles[i] = new TgaFile(textureFilepath + "\\" + filenames[i] + ".tga");
				cubemapData[i] = static_cast<void*>(&srcFiles[i]->GetImageData()[0]);
			}

			TextureCubemap* texture = m_gd.CreateTextureCubemap(srcFiles[0]->GetWidth(),
				srcFiles[0]->GetHeight(), cubemapData, 
				srcFiles[0]->GetBitsPerPixel() > 24 ? FORMAT_RGBA : FORMAT_RGBA,
				FILTERING_MIN_LINEAR | FILTERING_MAG_LINEAR, WRAP_REPEAT, 
				USAGE_IMMUTABLE, ACCESS_NO_ACCESS, false);

			m_texturesCubemap[textureFilepath] = texture;

			for (int i = 0; i < 6; i++)
			{
				delete srcFiles[i];
			}
			return texture;
		}
		else
		{
			return it->second;
		}
	}

	TextureCreator::~TextureCreator()
	{
		for (auto it = m_textures1D.begin(); it != m_textures1D.end(); it++)
		{
			delete it->second;
		}
		for (auto it = m_textures2D.begin(); it != m_textures2D.end(); it++)
		{
			delete it->second;
		}
		for (auto it = m_texturesCubemap.begin(); it != m_texturesCubemap.end(); it++)
		{
			delete it->second;
		}
	}
}