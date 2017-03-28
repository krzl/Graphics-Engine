#include "GBuffer.h"
#include "Textures\Texture2DDX.h"

namespace Kz
{
	class GBufferDX : public GBuffer
	{
		friend class GraphicsDeviceDX;

	private:

		Texture2DDX*                m_textures[TEXTURE_TYPE_COUNT];
		Texture2DDX*                m_depthTexture;

		ID3D11Device*               m_device;
		ID3D11DeviceContext*        m_context;
		GraphicsDeviceDX&           m_graphicsDevice;
		ID3D11RenderTargetView**    m_renderTargetViews;
		ID3D11DepthStencilView*     m_depthStencilView;

		GBufferDX(ID3D11Device* device, ID3D11DeviceContext* context,
			GraphicsDeviceDX& gd, int width, int height);

	public:

		~GBufferDX();

		virtual void                CopyToMainFramebuffer(Viewport& viewport);
		virtual void                Resize(int width, int height);
		virtual void                StartFrame();
		virtual void                BindGeomPass();
		virtual void                BindStencilPass();
		virtual void                BindLightPass();
		virtual void                BindFinalPass();;
		virtual void                Clear();
		virtual void                SetReadBuffer(TextureType textureType);
		virtual Texture&            GetTexture(TextureType textureType);

		void                        Init(int width, int height);
		void                        Dispose();
	};
}