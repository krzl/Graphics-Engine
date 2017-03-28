#pragma once

#include "Texture.h"
#include "../DXDirectives.h"

namespace Kz
{
	class TextureDX : public Texture
	{
		friend class GBuffer;

	private:

		ID3D11DepthStencilView*     m_depthStencilView = NULL;
		ID3D11ShaderResourceView*   m_shaderResourceView = NULL;
		ID3D11RenderTargetView*     m_renderTargetView = NULL;
		
		virtual void VTableFix(){}

	protected:

		ID3D11DeviceContext*        m_context;
		ID3D11Device*               m_device;

		unsigned int                m_sizeByte;

		ID3D11SamplerState*         m_sampler;

		DXGI_FORMAT                 GetFormat(DataFormat format);
		D3D11_USAGE                 GetUsage(Usage usage);
		unsigned int                GetCPUAccessFlag(CPUAccess access);
		void                        CreateDepthStencilView(ID3D11Resource* texture);
		void                        CreateShaderResourceView(ID3D11Resource* texture);
		void                        CreateRenderTargetView(ID3D11Resource* texture);
		void                        GenerateMips();
		unsigned int                GetSizeByte(DataFormat format);

		void                        ClearDepthStencilView();
		void                        ClearRenderTargetView();

		void                        ClearStencil();

		ID3D11RenderTargetView*     GetRenderTargetView();
		ID3D11DepthStencilView*     GetDepthStencilView();

		BYTE*                       GetTextureData(ID3D11Resource* texture);
		void                        UnmapTextureData(ID3D11Resource* texture);

	public:

		TextureDX(ID3D11Device* device, ID3D11DeviceContext* context, int filtering, int wrap);
		~TextureDX();

		void                        BindTextureVertexShader(int index);
		void                        BindTextureGeometryShader(int index);
		void                        BindTexturePixelShader(int index);
	};
}