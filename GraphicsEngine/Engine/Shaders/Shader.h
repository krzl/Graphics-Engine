#pragma once

#include <map>
#include <vector>

namespace Kz
{
	enum ShaderType
	{
		VERT_SHADER,
		GEOM_SHADER,
		FRAG_SHADER
	};

	class Buffer;
	struct UserBufferInfo;

	class Shader
	{
		friend class ShaderProgram;
		friend class Material;
		friend class Texture;

	protected:

		ShaderType						m_type;

		std::map<std::string, int>		m_predefBuffers;
		std::vector<UserBufferInfo*>	m_userBufferInfos;
		std::map<std::string, int>		m_textures;

		Shader() {}

	public:

		static bool						IsPredefinedBuffer(const std::string& name);
	};
}