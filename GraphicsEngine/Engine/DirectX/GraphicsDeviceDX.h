#pragma once

#include <vector>

#include "GraphicsDevice.h"
#include "DXDirectives.h"

namespace Kz
{
	class Texture2DDX;

	class GraphicsDeviceDX : public GraphicsDevice
	{
		friend class GBufferDX;

	private:

		unsigned int				m_msaa = 1;
		int							m_refreshRate = 60;
		bool						m_isWindowed = true;
		float						m_clearColor[4];

		ID3D11Device*				m_device = 0;
		ID3D11DeviceContext*		m_context = 0;
		IDXGISwapChain*				m_swapChain = 0;
		Texture2DDX*				m_depthTexture;
		ID3D11RenderTargetView*		m_renderTargetView = 0;
		ID3D11DepthStencilView*		m_depthStencilView = 0;
		UINT						m_msaaQuality;

		ID3D11BlendState*			m_blendOnSrcToInvSrc;
		ID3D11BlendState*			m_blendOnSrcToSrc;
		ID3D11BlendState*			m_blendOff;
		ID3D11DepthStencilState*	m_depthOn;
		ID3D11DepthStencilState*	m_depthOnStencilOnIncrDecr;
		ID3D11DepthStencilState*	m_depthOnWriteOff;
		ID3D11DepthStencilState*	m_depthOnWriteOffStencilOnIncrDecr;
		ID3D11DepthStencilState*	m_depthOffStencilOnKeep;
		ID3D11RasterizerState*		m_cullBack;
		ID3D11RasterizerState*		m_cullFront;
		ID3D11RasterizerState*		m_cullOffDepthClamp;

		void						CreateStates();

	public:

		static HWND					m_window;

		static void					SetHWnd(HWND hWnd);

		virtual void				Init(int width, int height);
		virtual void				OnResize(int width, int height);

		virtual Shader*				CreateShader(ShaderType type, const std::string& shaderName);

		virtual ShaderProgram*		CreateShaderProgram(std::vector<Shader*> shaders, 
										ShaderManager& shaderMgr);

		virtual ShaderProgram*		CreateShaderProgram(std::initializer_list<Shader*> shaders, 
										ShaderManager& shaderMgr);
		
		virtual Texture1D*			CreateTexture(int width, void* data, DataFormat format, 
										int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR, 
										int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT, 
										CPUAccess access = ACCESS_NO_ACCESS, bool generateMipmaps = false);
		
		virtual Texture2D*			CreateTexture(int width, int height, void* data, DataFormat format, 
										int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR, 
										int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT,
										CPUAccess access = ACCESS_NO_ACCESS, bool generateMipmaps = false);
		
		virtual TextureCubemap*		CreateTextureCubemap(int width, int height, void** data, 
										DataFormat format, int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR,
										int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT,
										CPUAccess access = ACCESS_NO_ACCESS, bool generateMipmaps = false);
		
		virtual GBuffer*			CreateGBuffer(int width, int height);

		virtual Buffer*				CreateBuffer(BufferType type, int dataSize, void* data = 0, 
										BufferUpdateFrequency updateFrequency = BUFFER_UPDATE_DYNAMIC, 
										int count = 1);
		
		virtual Mesh*				CreateMesh(Buffer& vertexBuffer, Buffer& indexBuffer);
		
		virtual void				SetupNextFrame();
		virtual void				SetViewport(int x, int y, int width, int height);
		virtual void				SetupAmbientPass();
		virtual void				SetupFinalPass();
		virtual void				SetupRenderSkybox();
		virtual void				FinalizeRenderSkybox();
		virtual void				SetupPointStencilPass();
		virtual void				SetupLightPassPoint();
		virtual void				SetupLightPass();
		virtual void				SetupShadowVolumeStencil();
		virtual void				FinalizeLightPass();
		virtual void				FinalizeLightPassPoint();
		virtual void				EndFinalPass();
	};
}