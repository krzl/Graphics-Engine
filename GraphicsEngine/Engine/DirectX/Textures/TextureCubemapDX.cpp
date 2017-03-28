#include "stdafx.h"
#include "TextureCubemapDX.h"

namespace Kz
{
	TextureCubemapDX::TextureCubemapDX(ID3D11Device* device, ID3D11DeviceContext* context,
		int width, int height, void** data, DataFormat format, int filtering, int wrap,
		Usage usage, CPUAccess access, bool generateMipmaps) :
			TextureDX(device, context, filtering, wrap)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width					= width;
		desc.Height					= height;
		desc.MipLevels				= 1;
		desc.ArraySize				= 6;
		desc.SampleDesc.Count		= 1;
		desc.SampleDesc.Quality		= 0;
		desc.Format					= GetFormat(format);
		desc.Usage					= GetUsage(usage);
		desc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags			= GetCPUAccessFlag(access);
		//desc.MiscFlags = (generateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0) | D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MiscFlags				= D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SUBRESOURCE_DATA dataInfo[6] = { 0 };

		m_sizeByte = GetSizeByte(format);

		m_width = width;
		m_height = height;

		for (int i = 0; i < 6; i++)
		{
			dataInfo[i].pSysMem				= data[i];
			dataInfo[i].SysMemPitch			= GetSizeByte(format) * width;
			dataInfo[i].SysMemSlicePitch	= m_sizeByte * width * height;
		}

		DXCALL_(device->CreateTexture2D(&desc, dataInfo, &m_texture));
		CreateShaderResourceView(m_texture);

		if (generateMipmaps)
			GenerateMips();
	}

	Vector4f TextureCubemapDX::GetTextureData(int x, int y, int z)
	{
		if (m_width > x && m_height > y)
		{
			float* data = (float*)TextureDX::GetTextureData(m_texture);

			Vector4f result(data[m_width * y * 4 + x * 4 + 0], data[m_width * y * 4 + x * 4 + 1], 
				data[m_width * y * 4 + x * 4 + 2], data[m_width * y * 4 + x * 4 + 3]);

			UnmapTextureData(m_texture);

			return result;
		}

		throw 0;
	}

	TextureCubemapDX::~TextureCubemapDX()
	{
		DX_RELEASE(m_texture);
	}
}