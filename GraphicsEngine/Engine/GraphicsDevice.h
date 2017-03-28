#pragma once

#include <string>

#include "Shaders\Shader.h"
#include "Texture.h"
#include "Buffer.h"


namespace Kz
{
	class ShaderProgram;
	class GBuffer;
	class Texture1D;
	class Texture2D;
	class TextureCubemap;
	class Mesh;
	class Buffer;
	class ShaderManager;

	enum SupportedAPI
	{
		DIRECTX
	};

	class GraphicsDevice
	{
	public:

		friend class SceneManager;
		friend class RenderSystem;

	private:

		static GraphicsDevice*  CreateGraphicsDevice(SupportedAPI chosenAPI);

	public:

		virtual void            Init(int width, int height) = 0;
		virtual void            OnResize(int width, int height) = 0;

		virtual Shader*         CreateShader(ShaderType type, const std::string& shaderName) = 0;

		virtual ShaderProgram*  CreateShaderProgram(std::vector<Shader*> shaders,
									ShaderManager& shaderMgr) = 0;

		virtual ShaderProgram*  CreateShaderProgram(std::initializer_list<Shader*> shaders, 
									ShaderManager& shaderMgr) = 0;

		virtual Texture1D*      CreateTexture(int width, void* data, DataFormat format, 
									int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR, 
									int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT, 
									CPUAccess access = ACCESS_NO_ACCESS, bool generateMipmaps = false) = 0;

		virtual Texture2D*      CreateTexture(int width, int height, void* data, DataFormat format, 
									int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR, 
									int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT, 
									CPUAccess access = ACCESS_NO_ACCESS, bool generateMipmaps = false) = 0;

		virtual TextureCubemap* CreateTextureCubemap(int width, int height, void** data, 
									DataFormat format, int filtering = FILTERING_MAG_LINEAR | FILTERING_MIN_LINEAR, 
									int wrap = WRAP_REPEAT, Usage usage = USAGE_DEFAULT, 
									CPUAccess access = ACCESS_NO_ACCESS, bool generateMipmaps = false) = 0;

		virtual GBuffer*        CreateGBuffer(int width, int height) = 0;

		virtual Buffer*         CreateBuffer(BufferType type, int dataSize, void* data = 0, 
									BufferUpdateFrequency updateFrequency = BUFFER_UPDATE_DYNAMIC, int count = 1) = 0;

		virtual Mesh*           CreateMesh(Buffer& vertexBuffer, Buffer& indexBuffer) = 0;

		virtual void            SetupNextFrame() = 0;
		virtual void            SetViewport(int x, int y, int width, int height) = 0;
		virtual void            SetupAmbientPass() = 0;
		virtual void            SetupFinalPass() = 0;
		virtual void            SetupRenderSkybox() = 0;
		virtual void            FinalizeRenderSkybox() = 0;
		virtual void            SetupPointStencilPass() = 0;
		virtual void            SetupLightPassPoint() = 0;
		virtual void            SetupLightPass() = 0;
		virtual void            SetupShadowVolumeStencil() = 0;
		virtual void            FinalizeLightPass() = 0;
		virtual void            FinalizeLightPassPoint() = 0;
		virtual void            EndFinalPass() = 0;

		Texture1D*              CreateRandomTexture1D(int width);
		Texture2D*              Create1x1Texture(float r, float g, float b);
	};
}