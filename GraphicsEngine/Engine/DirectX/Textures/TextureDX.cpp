#include "stdafx.h"
#include "TextureDX.h"
#include "SamplerManagerDX.h"

namespace Kz
{
	TextureDX::TextureDX(ID3D11Device* device, ID3D11DeviceContext* context, 
		int filtering, int wrap)
	{
		m_sampler = SamplerManager::GetSampler(device, context, filtering, wrap);
		m_context = context;
		m_device = device;
	}

	DXGI_FORMAT TextureDX::GetFormat(DataFormat format)
	{
		switch (format)
		{
		case FORMAT_R:
			return DXGI_FORMAT_R8_UNORM;
		case FORMAT_RG:
			return DXGI_FORMAT_R8G8_UNORM;
		case FORMAT_RGBX:
		case FORMAT_RGBA:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case FORMAT_RGB32F:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case FORMAT_RGBA32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case FORMAT_DEPTH24F_STENCIL8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	unsigned int TextureDX::GetSizeByte(DataFormat format){
		switch (format){
		case FORMAT_R:
			return 1;
		case FORMAT_RG:
			return 2;
		case FORMAT_RGBX:
		case FORMAT_RGBA:
			return 4;
		case FORMAT_RGB32F:
			return 12;
		case FORMAT_RGBA32F:
			return 16;
		case FORMAT_DEPTH24F_STENCIL8:
			return 4;
		default:
			return 4;
		}
	}

	D3D11_USAGE TextureDX::GetUsage(Usage access){
		switch (access){
		case USAGE_DEFAULT:
			return D3D11_USAGE_DEFAULT;
		case USAGE_IMMUTABLE:
			return D3D11_USAGE_IMMUTABLE;
		case USAGE_DYNAMIC:
			return D3D11_USAGE_DYNAMIC;
		case USAGE_RENDER_TARGET:
			return D3D11_USAGE_DEFAULT; //TODO: CHANGE
		case USAGE_DEPTH_STENCIL:
			return D3D11_USAGE_DEFAULT;
		default:
			__debugbreak(); //TODO: assertion
			return D3D11_USAGE_DEFAULT;
		}
	}
	
	unsigned int TextureDX::GetCPUAccessFlag(CPUAccess access)
	{
		switch (access)
		{
		case ACCESS_NO_ACCESS:
			return 0;
		case ACCESS_READ_ONLY:
			return D3D11_CPU_ACCESS_READ;
		case ACCESS_WRITE_ONLY:
			return D3D11_CPU_ACCESS_WRITE;
		case ACCESS_READ_WRITE:
			return D3D11_CPU_ACCESS_READ || D3D11_CPU_ACCESS_WRITE;
		default:
			__debugbreak(); //TODO: assertion
			return 0;
		}
	}

	void TextureDX::GenerateMips(){
		//m_context->GenerateMips(m_shaderResourceView); //TODO: mipmaps
	}

	void TextureDX::CreateShaderResourceView(ID3D11Resource* texture)
	{
		DXCALL_V(m_device->CreateShaderResourceView(texture, NULL, &m_shaderResourceView));
	}

	void TextureDX::CreateRenderTargetView(ID3D11Resource* texture)
	{
		DXCALL_V(m_device->CreateRenderTargetView(texture, NULL, &m_renderTargetView));
	}

	void TextureDX::CreateDepthStencilView(ID3D11Resource* texture)
	{
		DXCALL_V(m_device->CreateDepthStencilView(texture, NULL, &m_depthStencilView));
	}

	void TextureDX::BindTextureVertexShader(int index)
	{
		m_context->VSSetShaderResources(index, 1, &m_shaderResourceView);
		m_context->VSSetSamplers(index, 1, &m_sampler);
	}

	void TextureDX::BindTextureGeometryShader(int index)
	{
		m_context->GSSetShaderResources(index, 1, &m_shaderResourceView);
		m_context->GSSetSamplers(index, 1, &m_sampler);
	}

	void TextureDX::BindTexturePixelShader(int index)
	{
		m_context->PSSetShaderResources(index, 1, &m_shaderResourceView);
		m_context->PSSetSamplers(index, 1, &m_sampler);
	}

	void TextureDX::ClearDepthStencilView()
	{
		m_context->ClearDepthStencilView(m_depthStencilView, 
			D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void TextureDX::ClearStencil()
	{
		m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void TextureDX::ClearRenderTargetView()
	{
		float black[4] = { 0.f, 0.f, 0.f, 0.f };
		m_context->ClearRenderTargetView(m_renderTargetView, black);
	}

	ID3D11RenderTargetView* TextureDX::GetRenderTargetView()
	{
		return m_renderTargetView;
	}

	ID3D11DepthStencilView* TextureDX::GetDepthStencilView()
	{
		return m_depthStencilView;
	}

	BYTE* TextureDX::GetTextureData(ID3D11Resource* texture)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		m_context->Map(texture, 0, D3D11_MAP_READ, NULL, &mappedResource);

		BYTE* data = (BYTE*)mappedResource.pData;

		return data;
	}

	void TextureDX::UnmapTextureData(ID3D11Resource* texture)
	{
		m_context->Unmap(texture, 0);
	}

	TextureDX::~TextureDX()
	{
		DX_RELEASE(m_shaderResourceView);
		DX_RELEASE(m_renderTargetView);
	}
}