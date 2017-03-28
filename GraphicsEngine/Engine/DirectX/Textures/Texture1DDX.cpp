#include "stdafx.h"
#include "Texture1DDX.h"

namespace Kz
{
	Texture1DDX::Texture1DDX(ID3D11Device* device, ID3D11DeviceContext* context, int width,
		void* data, DataFormat format, int filtering, int wrap, Usage usage, CPUAccess access,
		bool generateMipmaps) :
		TextureDX(device, context, filtering, wrap)
	{
		D3D11_TEXTURE1D_DESC desc;
		desc.Width			= width;
		desc.MipLevels		= desc.ArraySize = 1;
		desc.Format			= GetFormat(format);
		desc.Usage			= GetUsage(usage);
		desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags	= GetCPUAccessFlag(access);
		desc.MiscFlags		= generateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		m_width		= width;
		m_sizeByte	= GetSizeByte(format);

		D3D11_SUBRESOURCE_DATA dataInfo = { 0 };
		dataInfo.pSysMem = data;

		DXCALL_(device->CreateTexture1D(&desc, &dataInfo, &m_texture));
		CreateShaderResourceView(m_texture);

		if (generateMipmaps)
			GenerateMips();
	}

	Vector4f Texture1DDX::GetTextureData(int x)
	{
		if (m_width > x)
		{
			float* data = (float*) TextureDX::GetTextureData(m_texture);
			Vector4f result(data[x * 4 + 0], data[x * 4 + 1], data[x * 4 + 2], data[x * 4 + 3]);
			UnmapTextureData(m_texture);

			return result;
		}

		throw 0; //TODO: add assertion
	}

	Texture1DDX::~Texture1DDX()
	{
		DX_RELEASE(m_texture);
	}
}