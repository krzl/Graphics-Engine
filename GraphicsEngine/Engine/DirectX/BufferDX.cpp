#include "stdafx.h"
#include "BufferDX.h"

namespace Kz
{
	BufferDX::BufferDX(ID3D11Device* device, ID3D11DeviceContext* context, BufferType type,
		BufferUpdateFrequency updateFrequency, void* data, int dataSize, int count)
	{
		m_device	= device;
		m_context	= context;
		int cpuAccessFlag = 0;

		switch (type)
		{
		case BUFFER_TYPE_VERTEX:
			m_bufferType = D3D11_BIND_VERTEX_BUFFER;
			break;
		case BUFFER_TYPE_INDEX:
			m_bufferType = D3D11_BIND_INDEX_BUFFER;
			break;
		case BUFFER_TYPE_SHADER_RESOURCE:
			m_bufferType = D3D11_BIND_CONSTANT_BUFFER;
			break;
		default:
			__debugbreak();
		}

		switch (updateFrequency)
		{
		case BUFFER_UPDATE_DYNAMIC:
			m_usage	= D3D11_USAGE_DYNAMIC;
			cpuAccessFlag = D3D11_CPU_ACCESS_WRITE;
			break;
		case BUFFER_UPDATE_STATIC:
			m_usage = D3D11_USAGE_IMMUTABLE;
			break;
		default:
			__debugbreak();
		}

		m_count		= count;
		m_stride	= dataSize / count;
		m_offset	= 0;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage			= m_usage;
		bufferDesc.ByteWidth		= dataSize;
		bufferDesc.BindFlags		= m_bufferType;
		bufferDesc.CPUAccessFlags	= cpuAccessFlag;
		bufferDesc.MiscFlags		= 0;

		D3D11_SUBRESOURCE_DATA* pSubresData = NULL;

		if (data != NULL)
		{
			D3D11_SUBRESOURCE_DATA initData;

			initData.pSysMem			= data;
			initData.SysMemPitch		= 0;
			initData.SysMemSlicePitch	= 0;

			pSubresData = &initData;
		}

		DXCALL_V(device->CreateBuffer(&bufferDesc, pSubresData, &m_buffer));
	}

	void BufferDX::BindBuffer()
	{
		switch (m_bufferType)
		{
		case D3D11_BIND_VERTEX_BUFFER:
			m_context->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &m_offset);
			break;
		case D3D11_BIND_INDEX_BUFFER:
			m_context->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
			break;
		}
	}

	void BufferDX::BindShaderResourceVertexShader(int slot)
	{
		m_context->VSSetConstantBuffers(slot, 1, &m_buffer);
	}

	void BufferDX::BindShaderResourceGeometryShader(int slot)
	{
		m_context->GSSetConstantBuffers(slot, 1, &m_buffer);
	}

	void BufferDX::BindShaderResourcePixelShader(int slot)
	{
		m_context->PSSetConstantBuffers(slot, 1, &m_buffer);
	}

	void BufferDX::SetData(void* data, int dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		DXCALL_V(m_context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource));

		void* cbData = resource.pData;
		memcpy(cbData, data, dataSize);
		m_context->Unmap(m_buffer, 0);
	}

	BufferDX::~BufferDX()
	{
		DX_RELEASE(m_buffer);
	}
}