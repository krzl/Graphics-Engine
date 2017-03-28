#include "stdafx.h"
#include "MeshDX.h"
#include "BufferDX.h"

namespace Kz
{
	MeshDX::MeshDX(ID3D11Device* device, ID3D11DeviceContext* context, 
		Buffer& vertexBuffer, Buffer& indexBuffer)
	{
		m_vertexBuffer	= &vertexBuffer;
		m_indexBuffer	= &indexBuffer;
		m_indexCount	= m_indexBuffer->GetCount();

		m_context = context;

		m_vertexBuffer->BindBuffer();
		m_indexBuffer->	BindBuffer();
	}

	void MeshDX::BindMesh()
	{
		m_vertexBuffer->BindBuffer();
		m_indexBuffer->	BindBuffer();
	}

	void MeshDX::Render()
	{
		BindMesh();
		m_context->DrawIndexed(m_indexCount, 0, 0);
	}

	MeshDX::~MeshDX()
	{
		delete m_vertexBuffer;
		delete m_indexBuffer;
	}
}