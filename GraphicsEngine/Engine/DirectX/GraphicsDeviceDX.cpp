#include "stdafx.h"
#include "GraphicsDeviceDX.h"
#include "Textures\Texture1DDX.h"
#include "Textures\TextureCubemapDX.h"
#include "Shaders\ShaderDX.h"
#include "Shaders\ShaderProgramDX.h"
#include "GBufferDX.h"
#include "BufferDX.h"
#include "MeshDX.h"
#include "DXDirectives.h"
#include "Utility\Logger.h"

namespace Kz
{
	HWND GraphicsDeviceDX::m_window = NULL;

	void GraphicsDeviceDX::Init(int width, int height)
	{
		m_clearColor[0] = 0.0f;
		m_clearColor[1] = 0.0f;
		m_clearColor[2] = 0.0f;
		m_clearColor[3] = 1.0f;

		UINT deviceFlags = 0;
#if defined(_DEBUG)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevel;
		DXCALL_V(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags,
			0, 0, D3D11_SDK_VERSION, &m_device, &featureLevel, &m_context));

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			Logger::Log("Direct3D Feature Level 11 unsupported.", Error);
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc.Width = 800; //TODO: Get dimensions and refresh rate of monitor
		swapChainDesc.BufferDesc.Height = 600;

		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		if (m_msaa > 1)
		{
			DXCALL_V(m_device->CheckMultisampleQualityLevels(
				DXGI_FORMAT_R8G8B8A8_UNORM, m_msaa, &m_msaaQuality));

			swapChainDesc.SampleDesc.Count = m_msaa;
			swapChainDesc.SampleDesc.Quality = m_msaaQuality - 1;
		}
		else
		{
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = m_window;
		swapChainDesc.Windowed = m_isWindowed;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		IDXGIDevice* dxgiDevice = 0;
		DXCALL_V(m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

		IDXGIAdapter* dxgiAdapter = 0;
		DXCALL_V(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

		IDXGIFactory* dxgiFactory = 0;
		DXCALL_V(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

		dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);

		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);

		DX_RELEASE(dxgiDevice);
		DX_RELEASE(dxgiAdapter);
		DX_RELEASE(dxgiFactory);

		CreateStates();

		OnResize(width, height);
	}

	void GraphicsDeviceDX::OnResize(int width, int height)
	{
		DX_RELEASE(m_renderTargetView);
		DX_RELEASE(m_depthStencilView);
		delete m_depthTexture;

		DXCALL_(m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D11Texture2D* backBuffer;
		DXCALL_(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(&backBuffer)));
		DXCALL_(m_device->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView));
		DX_RELEASE(backBuffer);

		m_depthTexture = reinterpret_cast<Texture2DDX*>(CreateTexture(width, height,
			NULL, FORMAT_DEPTH24F_STENCIL8, FILTERING_MAG_NEAREST || FILTERING_MIN_NEAREST, 
			WRAP_CLAMP, USAGE_DEPTH_STENCIL, ACCESS_NO_ACCESS, false));

		m_depthStencilView = m_depthTexture->GetDepthStencilView();

		m_context->RSSetState(m_cullBack);
		m_context->OMSetBlendState(m_blendOnSrcToInvSrc, NULL, 0xFFFFFFFF);
		m_context->OMSetDepthStencilState(m_depthOn, 0);
		m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	}


	void GraphicsDeviceDX::SetViewport(int x, int y, int width, int height)
	{
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = (FLOAT)x;
		viewport.TopLeftY = (FLOAT)y;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_context->RSSetViewports(1, &viewport);
	}

	void GraphicsDeviceDX::SetupNextFrame()
	{
	}

	Shader* GraphicsDeviceDX::CreateShader(ShaderType type, const std::string& shaderName)
	{
		return new ShaderDX(*this, m_device, m_context, type, shaderName);
	}

	ShaderProgram* GraphicsDeviceDX::CreateShaderProgram(std::vector<Shader*> shaders, 
		ShaderManager& shaderMgr)
	{
		return new ShaderProgramDX(m_device, m_context, shaders, shaderMgr);
	}

	ShaderProgram* GraphicsDeviceDX::CreateShaderProgram(std::initializer_list<Shader*> shaders, 
		ShaderManager& shaderMgr)
	{
		return new ShaderProgramDX(m_device, m_context, std::vector<Shader*>(shaders), shaderMgr);
	}

	Texture1D* GraphicsDeviceDX::CreateTexture(int width, void* data, DataFormat format, 
		int filtering, int wrap, Usage usage, CPUAccess access, bool generateMipmaps)
	{
		return dynamic_cast<Texture1D*>(
			new Texture1DDX(m_device, m_context, width, data, format,
			filtering, wrap, usage, access, generateMipmaps));
	}

	Texture2D* GraphicsDeviceDX::CreateTexture(int width, int height, void* data, 
		DataFormat format, int filtering, int wrap, Usage usage, 
		CPUAccess access, bool generateMipmaps)
	{
		return dynamic_cast<Texture2D*>(
			new Texture2DDX(m_device, m_context, width, height, data, format, 
			filtering, wrap, usage, access, generateMipmaps));
	}

	TextureCubemap* GraphicsDeviceDX::CreateTextureCubemap(int width, int height, void** data, 
		DataFormat format, int filtering, int wrap, Usage usage, 
		CPUAccess access, bool generateMipmaps)
	{
		return dynamic_cast<TextureCubemap*>(
			new TextureCubemapDX(m_device, m_context, width, height, data, format, 
			filtering, wrap, usage, access, generateMipmaps));
	}

	GBuffer* GraphicsDeviceDX::CreateGBuffer(int width, int height)
	{
		return new GBufferDX(m_device, m_context, *this, width, height);
	}

	Buffer* GraphicsDeviceDX::CreateBuffer(BufferType type, int dataSize, void* data, 
		BufferUpdateFrequency updateFrequency, int count)
	{
		return new BufferDX(m_device, m_context, type, updateFrequency, data, dataSize, count);
	}

	Mesh* GraphicsDeviceDX::CreateMesh(Buffer& vertexBuffer, Buffer& indexBuffer)
	{
		return new MeshDX(m_device, m_context, vertexBuffer, indexBuffer);
	}

	void GraphicsDeviceDX::SetupAmbientPass()
	{
		m_context->OMSetDepthStencilState(m_depthOnWriteOff, 0);
	}

	void GraphicsDeviceDX::SetupFinalPass()
	{
		//WRITE TO DEPTH ENABLED, DEPTH TEST ENABLED, STENCIL TEST DISABLED

		m_context->OMSetDepthStencilState(m_depthOn, 0);
		m_context->RSSetState(m_cullFront);
	}

	void GraphicsDeviceDX::EndFinalPass()
	{
		m_swapChain->Present(0, 0);
	}

	void GraphicsDeviceDX::SetupRenderSkybox()
	{
		m_context->OMSetDepthStencilState(m_depthOn, 0);
		m_context->RSSetState(m_cullFront);
	}

	void GraphicsDeviceDX::FinalizeRenderSkybox()
	{
		m_context->OMSetDepthStencilState(m_depthOn, 0);
		m_context->RSSetState(m_cullBack);
	}

	void GraphicsDeviceDX::SetupPointStencilPass()
	{
		//CURRENTLY UNUSED
		__debugbreak(); //TODO: assertion

		//DEPTH TEST ENABLED, CULLING DISABLED, CLEAR STENCIL, STENCIL FUNCTION ALWAYS, REFERENCE 0, MASK FF
		//STENCIL OP BACK INCR ON STENCIL PASS AND DEPTH FAIL, ELSE KEEP
		//STENCIL OP FRONT DECR, SAME AS ABOVE
	}

	void GraphicsDeviceDX::SetupLightPassPoint()
	{
		//CURRENTLY UNUSED
		__debugbreak(); //TODO: assertion

		//STENCIL FUNC NOTEQUAL REF 0 MASK FF
		//ENABLE CULLING, DISABLE DEPTH TEST
		//ENABLE BLENDING, BLEND FUNC ADD, ONE TO ONE
		//ENABLE CULL FACE, FRONT
	}

	void GraphicsDeviceDX::SetupLightPass()
	{
		m_context->RSSetState(m_cullBack);
		m_context->OMSetBlendState(m_blendOnSrcToSrc, NULL, 0xFFFFFFFF);
		m_context->OMSetDepthStencilState(m_depthOffStencilOnKeep, 0);
	}

	void GraphicsDeviceDX::SetupShadowVolumeStencil()
	{
		m_context->RSSetState(m_cullOffDepthClamp);
		m_context->OMSetDepthStencilState(m_depthOnWriteOffStencilOnIncrDecr, 0);
	}

	void GraphicsDeviceDX::FinalizeLightPass()
	{
		m_context->RSSetState(m_cullBack);
		m_context->OMSetBlendState(m_blendOff, NULL, 0xFFFFFFFF);
		m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_STENCIL, 0, 0);
	}

	void GraphicsDeviceDX::FinalizeLightPassPoint()
	{
		// CURRENTLY UNUSED
		__debugbreak(); //TODO: assertion

		m_context->OMSetBlendState(m_blendOff, NULL, 0xFFFFFFFF);
	}

	void GraphicsDeviceDX::SetHWnd(HWND hWnd)
	{
		m_window = hWnd;
	}

	void GraphicsDeviceDX::CreateStates()
	{
		D3D11_BLEND_DESC blendDesc = { 0 };

		// m_blendOnSrcToInvSrc
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		DXCALL_(m_device->CreateBlendState(&blendDesc, &m_blendOnSrcToInvSrc));

		// m_blendOnSrcToSrc
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		DXCALL_(m_device->CreateBlendState(&blendDesc, &m_blendOnSrcToSrc));

		// m_blendOff
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = false;

		DXCALL_(m_device->CreateBlendState(&blendDesc, &m_blendOff));
		

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = { 0 };

		// m_depthOn
		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilStateDesc.StencilEnable = false;

		DXCALL_(m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthOn));

		// m_depthOnStencilOnIncrDecr
		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilStateDesc.StencilEnable = true;
		depthStencilStateDesc.StencilReadMask = 0xFF;
		depthStencilStateDesc.StencilWriteMask = 0xFF;
		depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DXCALL_(m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthOnStencilOnIncrDecr));

		// m_depthOnWriteOff
		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilStateDesc.StencilEnable = false;

		DXCALL_(m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthOnWriteOff));

		// m_depthOnWriteOffStencilOnIncrDecr
		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilStateDesc.StencilEnable = true;
		depthStencilStateDesc.StencilReadMask = 0xFF;
		depthStencilStateDesc.StencilWriteMask = 0xFF;
		depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DXCALL_(m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthOnWriteOffStencilOnIncrDecr));

		// m_depthOffStencilOnKeep
		depthStencilStateDesc.DepthEnable = false;
		depthStencilStateDesc.StencilEnable = true;
		depthStencilStateDesc.StencilReadMask = 0xFF;
		depthStencilStateDesc.StencilWriteMask = 0xFF;
		depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		DXCALL_(m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthOffStencilOnKeep));
		

		D3D11_RASTERIZER_DESC rasterDesc;

		// m_cullBack
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0;
		rasterDesc.DepthClipEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0;

		DXCALL_(m_device->CreateRasterizerState(&rasterDesc, &m_cullBack));

		// m_cullFront
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0;
		rasterDesc.DepthClipEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0;

		DXCALL_(m_device->CreateRasterizerState(&rasterDesc, &m_cullFront));

		// m_cullOffDepthClamp
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.DepthClipEnable = false;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.DepthBias = 1;
		rasterDesc.DepthBiasClamp = 0;
		rasterDesc.SlopeScaledDepthBias = 0;

		DXCALL_(m_device->CreateRasterizerState(&rasterDesc, &m_cullOffDepthClamp));
	}
}