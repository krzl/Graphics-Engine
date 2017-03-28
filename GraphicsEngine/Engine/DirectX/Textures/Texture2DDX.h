#pragma once

#include "TextureDX.h"
#include "Texture.h"

namespace Kz
{
	class Texture2DDX : public Texture2D, public TextureDX
	{
		friend class GraphicsDeviceDX;
		friend class GBufferDX;

	private:

		int                 m_width;
		int                 m_height;

		ID3D11Texture2D*    m_texture;

		ID3D11Texture2D*    m_stagingTexture = NULL;

		Texture2DDX(ID3D11Device* device, ID3D11DeviceContext* context, int width,
			int height, void* data, DataFormat format,
			int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR, int wrap = WRAP_REPEAT,
			Usage usage = USAGE_DEFAULT, CPUAccess access = ACCESS_NO_ACCESS,
			bool generateMipmaps = false);

		~Texture2DDX();

	public:

		virtual Vector4f    GetTextureData(int x, int y);
	};
}