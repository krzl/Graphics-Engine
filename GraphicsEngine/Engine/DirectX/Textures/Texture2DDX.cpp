#include "stdafx.h"
#include "Texture2DDX.h"

namespace Kz
{
	Texture2DDX::Texture2DDX(ID3D11Device* device, ID3D11DeviceContext* context, int width,
		int height, void* data, DataFormat format, int filtering, int wrap,
		Usage usage, CPUAccess access, bool generateMipmaps) :
			TextureDX(device, context, filtering, wrap)
	{

		int bindFlags = D3D11_BIND_SHADER_RESOURCE;

		if (usage == USAGE_RENDER_TARGET)
		{
			bindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		if (usage == USAGE_DEPTH_STENCIL)
		{
			bindFlags = D3D11_BIND_DEPTH_STENCIL;
		}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = GetFormat(format);
		desc.Usage = GetUsage(usage);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = bindFlags;
		desc.CPUAccessFlags = GetCPUAccessFlag(access);
		//desc.MiscFlags = generateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
		desc.MiscFlags = 0;

		m_sizeByte = GetSizeByte(format);

		m_width = width;
		m_height = height;

		D3D11_SUBRESOURCE_DATA* subresData = NULL;

		if (data != NULL)
		{
			D3D11_SUBRESOURCE_DATA dataInfo = { 0 };

			dataInfo.pSysMem = data;
			dataInfo.SysMemPitch = width * m_sizeByte;

			subresData = &dataInfo;
		}

		DXCALL_(device->CreateTexture2D(&desc, subresData, &m_texture));

		if (usage == USAGE_RENDER_TARGET)
		{
			D3D11_TEXTURE2D_DESC stagingDesc;
			stagingDesc.Width = width;
			stagingDesc.Height = height;
			stagingDesc.MipLevels = stagingDesc.ArraySize = 1;
			stagingDesc.Format = GetFormat(format);
			stagingDesc.Usage = D3D11_USAGE_STAGING;
			stagingDesc.SampleDesc.Count = 1;
			stagingDesc.SampleDesc.Quality = 0;
			stagingDesc.BindFlags = 0;
			stagingDesc.CPUAccessFlags = GetCPUAccessFlag(access);
			stagingDesc.MiscFlags = 0;

			DXCALL_(device->CreateTexture2D(&stagingDesc, NULL, &m_stagingTexture));
		}

		if (usage != USAGE_DEPTH_STENCIL)
			CreateShaderResourceView(m_texture);
		else
			CreateDepthStencilView(m_texture);

		if (usage == USAGE_RENDER_TARGET)
			CreateRenderTargetView(m_texture);


		if (generateMipmaps)
			GenerateMips();
	}

	Vector4f Texture2DDX::GetTextureData(int x, int y)
	{
		if (m_width > x && m_height > y)
		{
			m_context->CopyResource(m_stagingTexture, m_texture);
			float* data = (float*)TextureDX::GetTextureData(m_stagingTexture);
			
			Vector4f result(data[m_width * y * 4 + x * 4 + 0], data[m_width * y * 4 + x * 4 + 1], 
				data[m_width * y * 4 + x * 4 + 2], data[m_width * y * 4 + x * 4 + 3]);

			UnmapTextureData(m_stagingTexture);

			return result;
		}
		throw 0;
	}

	Texture2DDX::~Texture2DDX()
	{
		DX_RELEASE(m_texture);
	}
}