#pragma once

namespace Kz
{
	enum BufferType
	{
		BUFFER_TYPE_VERTEX,
		BUFFER_TYPE_INDEX,
		BUFFER_TYPE_SHADER_RESOURCE
	};

	enum BufferUpdateFrequency
	{
		BUFFER_UPDATE_DYNAMIC,
		BUFFER_UPDATE_STATIC
	};

	class Buffer
	{
	public:

		int				m_count;

	public:

		virtual void	BindBuffer() = 0;
		virtual void	SetData(void* data, int dataSize) = 0;
		int				GetCount(){ return m_count; }
	};
}