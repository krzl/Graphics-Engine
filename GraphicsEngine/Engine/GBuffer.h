#pragma once

namespace Kz
{
	class Viewport;
	class Texture;

	enum TextureType
	{
		TEXTURE_TYPE_POSITION,
		TEXTURE_TYPE_DIFFUSE,
		TEXTURE_TYPE_NORMAL,
		TEXTURE_TYPE_COUNT
	};

	class GBuffer
	{
		friend class Renderer;
		friend class TechniqueManager;

	public:

		virtual void		CopyToMainFramebuffer(Viewport& viewport) = 0;
		virtual void		Resize(int width, int height) = 0;
		virtual void		StartFrame() = 0;
		virtual void		BindGeomPass() = 0;
		virtual void		BindStencilPass() = 0;
		virtual void		BindLightPass() = 0;
		virtual void		BindFinalPass() = 0;
		virtual void		Clear() = 0;
		virtual void		SetReadBuffer(TextureType textureType) = 0;
		virtual Texture&	GetTexture(TextureType textureType) = 0;
	};
}