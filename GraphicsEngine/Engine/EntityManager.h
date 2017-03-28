#pragma once

#include <unordered_map>

namespace Kz
{
	class Scene;
	class Entity;
	class Model;
	class Skybox;
	class GraphicsDevice;
	class TextureCreator;
	class Cube;
	class Sphere;
	class Plane;
	class ShaderProgram;
	class ShaderManager;
	class Material;

	class EntityManager
	{
		friend class SceneManager;
		friend class Entity;
		friend class AnimationSystem;

	private:

		Scene&							m_scene;
		GraphicsDevice&					m_gd;
		TextureCreator&					m_textureCreator;
		ShaderManager&					m_shaderManager;

		std::vector<AnimationSystem*>	m_animationSystems;
		std::map<std::string, Model*>	m_models;

		EntityManager(Scene& scene, GraphicsDevice& gd, TextureCreator& textureCreator, ShaderManager& srManager);

		void							RemoveEntity(Entity* entity);
		void							RemoveAnimatedEntity(AnimationSystem* entity);

	public:

		Material*						CreateMaterial(ShaderProgram& program);
		Entity*							CreateEntity(const std::string& modelName, ShaderProgram& shader);
		Entity*							CreateEntity(const std::string& modelName, Material& material);
		Skybox*							CreateSkybox(const std::string& textureFilename);
		Cube*							CreateCube(float size, ShaderProgram& shader);
		Cube*							CreateCube(float size, Material& material);
		Cube*							CreateCube(float x, float y, float z, ShaderProgram& shader);
		Cube*							CreateCube(float x, float y, float z, Material& material);
		Plane*							CreatePlane(float size, ShaderProgram& shader);
		Plane*							CreatePlane(float size, Material& material);
		Plane*							CreatePlane(float x, float z, ShaderProgram& shader);
		Plane*							CreatePlane(float x, float z, Material& material);
		Sphere*							CreateSphere(float size, ShaderProgram& shader);
		Sphere*							CreateSphere(float size, Material& material);
		Sphere*							CreateSphere(float x, float y, float z, ShaderProgram& shader);
		Sphere*							CreateSphere(float x, float y, float z, Material& material);

		void							Update(double timeElapsed);
	};
}