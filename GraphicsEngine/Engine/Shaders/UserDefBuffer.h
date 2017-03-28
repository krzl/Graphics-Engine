#pragma once

#include <map>

namespace Kz
{
	class Buffer;
	class GraphicsDevice;

	struct UserBufferInfo
	{
		int							bufferSize;
		int							bufferIndex;
		std::string					bufferName;
		std::map<std::string, int>	variablesOffsets;
	};

	class UserDefBuffer
	{
	private:

		bool						m_bufferChanged = false;
		Buffer*						m_buffer;
		void*						m_bufferData;
		UserBufferInfo&				m_bufferInfo;

	public:

		UserDefBuffer(GraphicsDevice& gd, UserBufferInfo& bufferInfo);
		~UserDefBuffer();

		Buffer&						GetBuffer();
		int							GetBufferIndex();
		void						SetShaderVariable(const std::string& variableName,
										void* data, int dataSize);
		void						UpdateBuffer();
	};
}