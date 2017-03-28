#include "stdafx.h"
#include "GBufferDX.h"
#include "GraphicsDeviceDX.h"
#include "Viewport.h"

namespace Kz
{
	GBufferDX::GBufferDX(ID3D11Device* device, ID3D11DeviceContext* context,
		GraphicsDeviceDX& gd, int width, int height) : 
		m_graphicsDevice(gd),
		m_device(device),
		m_context(context)
	{
		m_renderTargetViews = new ID3D11RenderTargetView*[TEXTURE_TYPE_COUNT];

		Init(width, height);
	}

	void GBufferDX::Init(int width, int height)
	{
		for (int i = 0; i < TEXTURE_TYPE_COUNT; i++)
		{
			m_textures[i] = reinterpret_cast<Texture2DDX*>(m_graphicsDevice.CreateTexture(
				width, height, NULL, FORMAT_RGBA32F, FILTERING_MAG_NEAREST || FILTERING_MIN_NEAREST, 
				WRAP_CLAMP, USAGE_RENDER_TARGET, ACCESS_READ_ONLY, false));

			m_renderTargetViews[i] = m_textures[i]->GetRenderTargetView();
		}

		m_depthTexture =
			reinterpret_cast<Texture2DDX*>(m_graphicsDevice.CreateTexture(width,
			height, NULL, FORMAT_DEPTH24F_STENCIL8, FILTERING_MAG_NEAREST || FILTERING_MIN_NEAREST,
			WRAP_CLAMP, USAGE_DEPTH_STENCIL, ACCESS_NO_ACCESS, false));

		m_depthStencilView = m_depthTexture->GetDepthStencilView();
	}

	void GBufferDX::StartFrame()
	{
		float black[4] = { 0.f, 0.f, 0.f, 0.f };
		m_context->ClearRenderTargetView(m_graphicsDevice.m_renderTargetView, black);
		m_context->ClearDepthStencilView(m_graphicsDevice.m_depthStencilView, 
			D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

		BindGeomPass();
		Clear();
	}

	void GBufferDX::BindGeomPass()
	{
		ID3D11ShaderResourceView* nullSRV[TEXTURE_TYPE_COUNT];

		for (int i = 0; i < TEXTURE_TYPE_COUNT; i++)
			nullSRV[i] = nullptr;

		m_context->PSSetShaderResources(0, TEXTURE_TYPE_COUNT, nullSRV);
		m_context->OMSetRenderTargets(TEXTURE_TYPE_COUNT, m_renderTargetViews,
			m_graphicsDevice.m_depthStencilView);
	}

	void GBufferDX::BindStencilPass()
	{
		m_context->OMSetRenderTargets(0, NULL, m_graphicsDevice.m_depthStencilView);
	}

	void GBufferDX::BindLightPass()
	{
		m_context->OMSetRenderTargets(1, &m_graphicsDevice.m_renderTargetView,
			m_graphicsDevice.m_depthStencilView);
	}

	void GBufferDX::BindFinalPass()
	{
		m_context->OMSetRenderTargets(1, &m_graphicsDevice.m_renderTargetView,
			m_graphicsDevice.m_depthStencilView);
	}

	void GBufferDX::SetReadBuffer(TextureType textureType)
	{
	}

	void GBufferDX::Clear()
	{
		for (int i = 0; i < TEXTURE_TYPE_COUNT; i++)
			m_textures[i]->ClearRenderTargetView();
		
		m_depthTexture->ClearDepthStencilView();
	}

	void GBufferDX::Resize(int width, int height)
	{
		Dispose();
		Init(width, height);
	}

	void GBufferDX::CopyToMainFramebuffer(Viewport& viewport)
	{
	}

	Texture& GBufferDX::GetTexture(TextureType textureType)
	{
		return reinterpret_cast<Texture&>(*m_textures[textureType]);
	}

	void GBufferDX::Dispose()
	{
		for (int i = 0; i < TEXTURE_TYPE_COUNT; i++)
			delete m_textures[i];

		delete m_depthTexture;
	}

	GBufferDX::~GBufferDX()
	{
		Dispose();
	}
}