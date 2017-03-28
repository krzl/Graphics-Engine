#pragma once

#include <vector>
#include <map>

#include "Math\Matrix.h"
#include "Shader.h"

#define MAX_BONES 64

namespace Kz
{
	class GBuffer;
	class Texture;
	class Buffer;
	class GraphicsDevice;
	class ShaderProgram;
	class Shader;

	enum PredefShader
	{
		ANIM_SHADOW_VOLUME_VERT,
		BASIC_VERT,
		LIGHT_PASS_FRAG,
		NULL_VERT,
		NULL_FRAG,
		SHADOW_VOLUME_VERT,
		SHADOW_VOLUME_GEOM,
		SKYBOX_VERT,
		SKYBOX_FRAG,
		PREDEF_SHADER_COUNT
	};

	enum PredefShaderProgram
	{
		ANIM_SHADOW_VOLUME_PROGRAM,
		BASIC_PROGRAM,
		LIGHT_PASS_PROGRAM,
		SHADOW_VOLUME_PROGRAM,
		SKYBOX_PROGRAM,
		PREDEF_SHADER_PROGRAM_COUNT
	};

	struct RenderPerFrameInfo
	{
		Matrix4f    pvMatrix;
		Vector3f    cameraPos;
		float       spacing0;
		Vector3f    cameraUp;
		float       spacing1;
		Vector2f    screenSize;
		float       deltaTime;
		float       spacing2;
	};

	struct RenderPerEntityInfo
	{
		Matrix4f    pvmMatrix;
		Matrix4f    modelMatrix;
		Vector3f    scaleVector;
		int         entityID;
	};

	struct RenderPerLightInfo
	{
		Vector3f    position;
		int         type;
		Vector4f    ambient;
		Vector4f    diffuse;
		Vector4f    specular;
		Vector3f    direction;
		float       cutoff;
		Vector3f    attenuation;
		int         spotExponent;
	};

	struct RenderPerSkinningInfo
	{
		Matrix4f    bonesData[MAX_BONES];
	};

	enum BufferName
	{
		BUFFER_PER_FRAME,
		BUFFER_PER_ENTITY,
		BUFFER_PER_LIGHT,
		BUFFER_PER_SKINNING,
		BUFFER_COUNT
	};

	class ShaderManager
	{
		friend class RenderSystem;

	private:

		Buffer*                         m_buffers[BUFFER_COUNT];
		GraphicsDevice*                 m_gd;

		std::vector<Shader*>            m_predefShaders;
		std::vector<ShaderProgram*>     m_predefShaderPrograms;
		std::vector<Material*>          m_materials;

		void                            Init(GraphicsDevice& gd);

	public:

		Buffer&                         GetBuffer(const std::string& name);
		Buffer&                         GetBuffer(BufferName buffer);
		BufferName                      GetBufferType(const std::string& name);
		void                            SetBuffer(BufferName buffer, void* data, int dataSize);

		Shader*                         CreateShader(ShaderType type, std::string filename);
		ShaderProgram*                  CreateShaderProgram(std::initializer_list<Shader*> shaders);

		Material*                       GetPredefMaterial(PredefShaderProgram shaderProgram);

		~ShaderManager();
	};
}