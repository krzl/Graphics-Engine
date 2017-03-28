#include "stdafx.h"
#include "ShaderProgram.h"
#include "Shader.h"

namespace Kz
{
	ShaderManager* ShaderProgram::shaderManager = NULL;

	ShaderProgram::ShaderProgram(std::vector<Shader*>& shaders)
	{
		m_shaders = shaders;
	}
}