#pragma once

#define AMBIENT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 3

#include "Math\Matrix.h"
#include "Shaders\ShaderManager.h"

namespace Kz
{
	class RenderSystem;
	class Entity;
	class EntityManager;

	class Light
	{
		friend class SceneManager;
		friend class RenderSystem;
		friend class SceneNode;

	private:

		static EntityManager*	GetEntityManager();
		static void				SetEntityManager(EntityManager& modelManager);

	protected:

		SceneNode*				m_sceneNode;
		RenderPerLightInfo		m_lightInfo;

		static EntityManager*	m_entityManager;

		Light(){}

		void					SetNode(SceneNode* sceneNode);
		void					SetLightPosition(RenderSystem& renderSystem);
		virtual	void			LightPass(RenderSystem& renderSystem) = 0;

	public:

		~Light();

		const Vector4f&			GetDiffuse() const;
		const Vector4f&			GetSpecular() const;
		virtual void			SetDiffuse(Vector4f& diffuseColor);
		virtual void			SetSpecular(Vector4f& specularColor);
	};
}