#pragma once

#include <vector>

namespace Kz
{
	class ShaderManager;
	class Shader;

	class ShaderProgram
	{
		friend class ShaderManager;
		friend class Material;

	private:

		static ShaderManager* shaderManager;

	protected:

		std::vector<Shader*> m_shaders;

		ShaderProgram(std::vector<Shader*>& shaders);

	public:

		virtual void Enable(ShaderManager& srManager, Material& material) = 0;
	};
}