#include "Mesh.h"
#include "DXDirectives.h"

namespace Kz
{
	class Buffer;

	class MeshDX : public Mesh
	{
		friend class GraphicsDeviceDX;

	private:

		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_context;
		int						m_indexCount;
		Buffer*					m_vertexBuffer;
		Buffer*					m_indexBuffer;

		MeshDX(ID3D11Device* device, ID3D11DeviceContext* context, 
			Buffer& vertexBuffer, Buffer& indexBuffer);

	public:

		~MeshDX();

		virtual void			BindMesh();
		virtual void			Render();
	};
}