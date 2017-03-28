#pragma once

#include <vector>
#include <map>

namespace Kz
{
	class Shader;
	class ShaderProgram;
	class UserDefBuffer;
	class ShaderManager;
	class Texture;
	class GraphicsDevice;
	class Model;

	struct ShaderVariablesData
	{
		Shader*                             shader;
		std::vector<UserDefBuffer*>         buffers;
		std::map<std::string, Texture*>     textures;
	};

	class Material
	{
	private:

		ShaderProgram&                      m_program;

		std::vector<ShaderVariablesData*>   m_shadersVariables;

	public:

		Material(Material& material);
		Material(GraphicsDevice& gd, ShaderProgram& program);
		Material(GraphicsDevice& gd, ShaderProgram& program, Model& model);
		~Material();

		void                                Bind(ShaderManager& srManager);

		ShaderProgram&                      GetShaderProgram();
		ShaderVariablesData&                GetShaderVariablesData(Shader* shader);
		void                                SetTexture(const std::string& textureName, Texture* texture);
		void                                SetShaderVariable(const std::string& variable, 
												void* data, int dataSize);
	};
}