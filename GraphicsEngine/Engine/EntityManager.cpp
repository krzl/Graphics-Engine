#include "stdafx.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Model.h"
#include "Skybox.h"
#include "GraphicsDevice.h"
#include "TextureCreator.h"
#include "Primitives\Cube.h"
#include "Primitives\Plane.h"
#include "Primitives\Sphere.h"
#include "Shaders\ShaderProgram.h"
#include "AnimationSystem.h"

namespace Kz
{
	EntityManager::EntityManager(Scene& scene, GraphicsDevice& gd,
		TextureCreator& textureCreator, ShaderManager& srManager) :
		m_scene(scene),
		m_gd(gd),
		m_textureCreator(textureCreator),
		m_shaderManager(srManager)
	{}

	Material* EntityManager::CreateMaterial(ShaderProgram& program)
	{
		return new Material(m_gd, program);
	}

	Entity* EntityManager::CreateEntity(const std::string& modelName, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find(modelName);
		Entity* entity = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model(modelName, m_gd, m_textureCreator);
			m_models[modelName] = model;

			entity = new Entity(*model, m_gd, shader, *this, m_scene);

			if (model->IsRigged())
			{
				m_animationSystems.push_back(&entity->GetAnimationSystem());
			}
		}
		else
		{
			entity = new Entity(*(it->second), m_gd, shader, *this, m_scene);

			if (it->second->IsRigged())
			{
				m_animationSystems.push_back(&entity->GetAnimationSystem());
			}
		}

		return entity;
	}

	Entity* EntityManager::CreateEntity(const std::string& modelName, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find(modelName);
		Entity* entity = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model(modelName, m_gd, m_textureCreator);
			m_models[modelName] = model;

			entity = new Entity(*model, m_gd, material, *this, m_scene);
		}
		else
		{
			entity = new Entity(*(it->second), m_gd, material, *this, m_scene);
		}

		return entity;
	}

	Skybox* EntityManager::CreateSkybox(const std::string& textureFilename)
	{
		return new Skybox(*this, m_textureCreator, textureFilename,
			m_shaderManager.GetPredefMaterial(SKYBOX_PROGRAM)->GetShaderProgram());
	}

	Cube* EntityManager::CreateCube(float size, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Cube\\Cube.mesh.xml");
		Cube* cube = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Cube\\Cube.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Cube\\Cube.mesh.xml"] = model;

			cube = new Cube(*model, size, size, size, m_gd, shader);
		}
		else
		{
			cube = new Cube(*(it->second), size, size, size, m_gd, shader);
		}

		return cube;
	}

	Cube* EntityManager::CreateCube(float size, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Cube\\Cube.mesh.xml");
		Cube* cube = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Cube\\Cube.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Cube\\Cube.mesh.xml"] = model;

			cube = new Cube(*model, size, size, size, m_gd, material);
		}
		else
		{
			cube = new Cube(*(it->second), size, size, size, m_gd, material);
		}

		return cube;
	}

	Cube* EntityManager::CreateCube(float x, float y, float z, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Cube\\Cube.mesh.xml");
		Cube* cube = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Cube\\Cube.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Cube\\Cube.mesh.xml"] = model;

			cube = new Cube(*model, x, y, z, m_gd, shader);
		}
		else
		{
			cube = new Cube(*(it->second), x, y, z, m_gd, shader);
		}

		return cube;
	}

	Cube* EntityManager::CreateCube(float x, float y, float z, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Cube\\Cube.mesh.xml");
		Cube* cube = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Cube\\Cube.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Cube\\Cube.mesh.xml"] = model;

			cube = new Cube(*model, x, y, z, m_gd, material);
		}
		else
		{
			cube = new Cube(*(it->second), x, y, z, m_gd, material);
		}

		return cube;
	}

	Plane* EntityManager::CreatePlane(float size, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Plane\\Plane.mesh.xml");
		Plane* plane = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Plane\\Plane.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Plane\\Plane.mesh.xml"] = model;

			plane = new Plane(*model, size, size, m_gd, shader);
		}
		else
		{
			plane = new Plane(*(it->second), size, size, m_gd, shader);
		}

		return plane;
	}

	Plane* EntityManager::CreatePlane(float size, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Plane\\Plane.mesh.xml");
		Plane* plane = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Plane\\Plane.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Plane\\Plane.mesh.xml"] = model;

			plane = new Plane(*model, size, size, m_gd, material);
		}
		else
		{
			plane = new Plane(*(it->second), size, size, m_gd, material);
		}

		return plane;
	}

	Plane* EntityManager::CreatePlane(float x, float z, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Plane\\Plane.mesh.xml");
		Plane* plane = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Plane\\Plane.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Plane\\Plane.mesh.xml"] = model;

			plane = new Plane(*model, x, z, m_gd, shader);
		}
		else
		{
			plane = new Plane(*(it->second), x, z, m_gd, shader);
		}

		return plane;
	}

	Plane* EntityManager::CreatePlane(float x, float z, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Plane\\Plane.mesh.xml");
		Plane* plane = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Plane\\Plane.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Plane\\Plane.mesh.xml"] = model;

			plane = new Plane(*model, x, z, m_gd, material);
		}
		else
		{
			plane = new Plane(*(it->second), x, z, m_gd, material);
		}

		return plane;
	}

	Sphere* EntityManager::CreateSphere(float size, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml");
		Sphere* sphere = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml"] = model;

			sphere = new Sphere(*model, size, size, size, m_gd, shader);
		}
		else
		{
			sphere = new Sphere(*(it->second), size, size, size, m_gd, shader);
		}

		return sphere;
	}

	Sphere* EntityManager::CreateSphere(float size, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml");
		Sphere* sphere = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml"] = model;

			sphere = new Sphere(*model, size, size, size, m_gd, material);
		}
		else
		{
			sphere = new Sphere(*(it->second), size, size, size, m_gd, material);
		}

		return sphere;
	}

	Sphere* EntityManager::CreateSphere(float x, float y, float z, ShaderProgram& shader)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml");
		Sphere* sphere = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml"] = model;

			sphere = new Sphere(*model, x, y, z, m_gd, shader);
		}
		else
		{
			sphere = new Sphere(*(it->second), x, y, z, m_gd, shader);
		}

		return sphere;
	}

	Sphere* EntityManager::CreateSphere(float x, float y, float z, Material& material)
	{
		std::map<std::string, Model*>::iterator it = m_models.find("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml");
		Sphere* sphere = NULL;

		if (it == m_models.end())
		{
			Model* model = new Model("..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml", m_gd, m_textureCreator);
			m_models["..\\Models\\Primitives\\Sphere\\Sphere.mesh.xml"] = model;

			sphere = new Sphere(*model, x, y, z, m_gd, material);
		}
		else
		{
			sphere = new Sphere(*(it->second), x, y, z, m_gd, material);
		}

		return sphere;
	}

	void EntityManager::Update(double timeElapsed)
	{
		for (std::vector<AnimationSystem*>::iterator it = m_animationSystems.begin(); it != m_animationSystems.end();){
			(**it).Update((float) timeElapsed);
			it++;
		}
	}

	void EntityManager::RemoveAnimatedEntity(AnimationSystem* animationSystem)
	{
		std::vector<AnimationSystem*>::iterator it = std::find(m_animationSystems.begin(), m_animationSystems.end(), animationSystem);
		if (it != m_animationSystems.end())
		{
			m_animationSystems.erase(it);
		}
	}
}