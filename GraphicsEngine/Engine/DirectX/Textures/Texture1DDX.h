#pragma once

#include "TextureDX.h"
#include "Texture.h"

namespace Kz
{
	class Texture1DDX : public Texture1D, public TextureDX
	{
		friend class GraphicsDeviceDX;
		friend class GBufferDX;

	private:

		int					m_width;

		ID3D11Texture1D*	m_texture;

		Texture1DDX(ID3D11Device* device, ID3D11DeviceContext* context, int width,
			void* data, DataFormat format, int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR,
			int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT, CPUAccess access = ACCESS_NO_ACCESS, 
			bool generateMipmaps = false);

		~Texture1DDX();

	public:

		virtual Vector4f	GetTextureData(int x);
	};
}