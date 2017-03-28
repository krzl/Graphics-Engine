#include "stdafx.h"
#include "UserDefBuffer.h"
#include "GraphicsDevice.h"

namespace Kz
{
	UserDefBuffer::UserDefBuffer(GraphicsDevice& gd, UserBufferInfo& bufferInfo) :
		m_bufferInfo(bufferInfo)
	{
		m_bufferData	= ::operator new (bufferInfo.bufferSize);
		m_buffer		= gd.CreateBuffer(BUFFER_TYPE_SHADER_RESOURCE, 
			bufferInfo.bufferSize, NULL, BUFFER_UPDATE_DYNAMIC);
	}

	Buffer& UserDefBuffer::GetBuffer()
	{
		return *m_buffer;
	}

	int UserDefBuffer::GetBufferIndex()
	{
		return m_bufferInfo.bufferIndex;
	}

	void UserDefBuffer::SetShaderVariable(const std::string& variableName, void* data, int dataSize)
	{
		std::map<std::string, int>::iterator it = m_bufferInfo.variablesOffsets.find(variableName);

		if (it == m_bufferInfo.variablesOffsets.end())
		{
			m_bufferChanged = true;
			memcpy(static_cast<char*>(m_bufferData) + it->second, data, dataSize);
		}
	}

	void UserDefBuffer::UpdateBuffer()
	{
		if (m_bufferChanged)
		{
			m_buffer->BindBuffer();
			m_buffer->SetData(m_bufferData, m_bufferInfo.bufferSize);
			
			m_bufferChanged = false;
		}
	}

	UserDefBuffer::~UserDefBuffer()
	{
		::operator delete(m_bufferData);
	}
}