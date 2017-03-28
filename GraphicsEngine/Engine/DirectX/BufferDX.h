#pragma once

#include "Buffer.h"
#include "DXDirectives.h"

namespace Kz
{
	class BufferDX : public Buffer
	{
		friend class GraphicsDeviceDX;

	private:

		unsigned int			m_stride;
		unsigned int			m_offset;

		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_context;
		ID3D11Buffer*			m_buffer;
		D3D11_USAGE				m_usage;
		D3D11_BIND_FLAG			m_bufferType;

		BufferDX(ID3D11Device* device, ID3D11DeviceContext* context, BufferType type,
			BufferUpdateFrequency updateFrequency, void* data, int dataSize, int count);

	public:

		~BufferDX();

		virtual void	BindBuffer();
		void			BindShaderResourceVertexShader(int slot);
		void			BindShaderResourceGeometryShader(int slot);
		void			BindShaderResourcePixelShader(int slot);
		virtual void	SetData(void* data, int dataSize);
	};
}