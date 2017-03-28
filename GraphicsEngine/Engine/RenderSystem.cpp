#include "stdafx.h"
#include "RenderSystem.h"
#include "Entity.h"
#include "Lights\Light.h"
#include "Lights\AmbientLight.h"
#include "Scene.h"
#include "Skybox.h"
#include "Viewport.h"
#include "GraphicsDevice.h"

#include "DirectX\Textures\Texture2DDX.h"

namespace Kz
{
	RenderSystem::RenderSystem(GraphicsDevice* gd) :
		m_gd(*gd)
	{
	
	}

	void RenderSystem::Init(int width, int height)
	{
		m_gd.Init(width, height);
		m_shaderManager.Init(m_gd);
	}

	void RenderSystem::SetScene(Scene* scene)
	{
		m_scene = scene;
	}

	Matrix4f& RenderSystem::GetPVMatrix()
	{
		return m_frameInfo.pvMatrix;
	}

	void RenderSystem::AttachEntity(Entity* entity)
	{
		m_renderList.push_back(entity);
	}

	void RenderSystem::AttachLight(Light* light)
	{
		m_lightsList.push_back(light);
	}

	void RenderSystem::DetachEntity(Entity* entity)
	{
		std::vector<Entity*>::iterator it = std::find(
			m_renderList.begin(), m_renderList.end(), entity);

		if (it != m_renderList.end())
			m_renderList.erase(it);
	}

	void RenderSystem::DetachLight(Light* light)
	{
		std::vector<Light*>::iterator it = std::find(
			m_lightsList.begin(), m_lightsList.end(), light);
		
		if (it != m_lightsList.end())
			m_lightsList.erase(it);
	}

	void RenderSystem::SetDeltaTime(float timeElapsed)
	{
		m_frameInfo.deltaTime = timeElapsed;
	}

	void RenderSystem::Render(Viewport& viewport)
	{
		m_gd.SetViewport(viewport.GetX(), viewport.GetY(), 
			viewport.GetWidth(), viewport.GetHeight());

		Matrix4f viewMatrix = viewport.m_camera.GetLookAtMatrix();
		Matrix4f projectionMatrix = 
			Matrix4f::ProjectionMatrix(viewport.GetAspectRatio(), 70.0f, 100.0f, 1.f);

		m_currentGBuffer = &viewport.GetGBuffer();

		m_frameInfo.pvMatrix   = projectionMatrix * viewMatrix;
		m_frameInfo.cameraUp   = viewport.m_camera.GetUp();
		m_frameInfo.cameraPos  = viewport.m_camera.GetPosition();
		m_frameInfo.screenSize = Vector2f((float)viewport.GetWidth(), (float)viewport.GetHeight());

		Texture* texture = &m_currentGBuffer->GetTexture(TEXTURE_TYPE_DIFFUSE);

		m_shaderManager.GetPredefMaterial(LIGHT_PASS_PROGRAM)->SetTexture("positionMap", &m_currentGBuffer->GetTexture(TEXTURE_TYPE_POSITION));
		m_shaderManager.GetPredefMaterial(LIGHT_PASS_PROGRAM)->SetTexture("colorMap", &m_currentGBuffer->GetTexture(TEXTURE_TYPE_DIFFUSE));
		m_shaderManager.GetPredefMaterial(LIGHT_PASS_PROGRAM)->SetTexture("normalMap", &m_currentGBuffer->GetTexture(TEXTURE_TYPE_NORMAL));
		m_shaderManager.SetBuffer(BUFFER_PER_FRAME, (void*)&m_frameInfo, sizeof(RenderPerFrameInfo));

		m_currentGBuffer->StartFrame();
		GeomPass();
		LightPass();
		FinalPass(viewport);

		m_currentGBuffer->CopyToMainFramebuffer(viewport);
	}

	void RenderSystem::GeomPass()
	{
		for (Entity* entity : m_renderList)
		{
			entity->PreRender(*this);
			entity->Render();
		}
	}

	void RenderSystem::LightPass()
	{
		for (Light* light : m_lightsList)
		{
			light->LightPass(*this);
		}
		if (m_scene->m_ambientLight)
		{
			m_gd.SetupAmbientPass();
			m_scene->m_ambientLight->LightPass(*this);
		}
	}

	void RenderSystem::ShadowPass()
	{
		for (Entity* entity : m_renderList)
		{
			if (entity->IsCastingShadow())
			{
				entity->PreShadow(*this);
				entity->Render();
			}
		}
	}

	void RenderSystem::FinalPass(Viewport& viewport)
	{
		m_gd.SetupFinalPass();

		if (m_scene->m_skybox)
		{
			RenderSkybox(viewport);
		}

		m_gd.FinalizeRenderSkybox();

		viewport.GetGBuffer().BindFinalPass();

		m_gd.EndFinalPass();
	}

	void RenderSystem::RenderSkybox(Viewport& viewport)
	{
		m_gd.SetupRenderSkybox();

		m_scene->m_skybox->SetViewport(viewport);
		m_scene->m_skybox->PreRender(*this);
		m_scene->m_skybox->Render();
	}

	void RenderSystem::SetupPointStencilPass()
	{
		m_currentGBuffer->BindStencilPass();
		m_gd.SetupPointStencilPass();
	}

	void RenderSystem::SetupLightPassPoint()
	{
		m_currentGBuffer->BindLightPass();
		m_gd.SetupLightPassPoint();
	}

	void RenderSystem::SetupLightPass()
	{
		m_currentGBuffer->BindLightPass();
		m_gd.SetupLightPass();
	}

	void RenderSystem::SetupShadowVolumeStencil()
	{
		m_currentGBuffer->BindStencilPass();
		m_gd.SetupShadowVolumeStencil();
	}

	void RenderSystem::FinalizeLightPass()
	{
		m_gd.FinalizeLightPass();
	}

	void RenderSystem::FinalizeLightPassPoint()
	{
		m_gd.FinalizeLightPassPoint();
	}
}