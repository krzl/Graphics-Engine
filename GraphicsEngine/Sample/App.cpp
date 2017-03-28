#include "App.h"
#include "Engine\Lights\PointLight.h"
#include "Engine\Lights\DirectionalLight.h"
#include "Engine\Lights\SpotLight.h"
#include "Engine\Viewport.h"
#include "Engine\Entity.h"
#include "Engine\AnimationSystem.h"
#include "Engine\Primitives\Plane.h"
#include "Commands.h"

void App::OnInit()
{
	m_sceneManager.GetScene().SetAmbientLight(Vector4f(0.3f, 0.3f, 0.3f, 1.0));

	Kz::PointLight* light = m_sceneManager.CreatePointLight();
	Kz::DirectionalLight* dirLight = m_sceneManager.CreateDirectionalLight();

	Kz::Skybox* skybox = m_sceneManager.GetEntityManager().CreateSkybox("..\\Skyboxes\\stormy_days");

	m_sceneManager.GetScene().AttachSkybox(skybox);

	light->SetAttenuation(Vector3f(0.f, 0.f, 0.000002f));
	light->SetDiffuse(Vector4f(0.6f, 0.6f, 0.6f, 1.0f));
	light->SetSpecular(Vector4f(0.8f, 0.8f, 0.8f, 1.0f));

	dirLight->SetDiffuse(Vector4f(0.3f, 0.3f, 0.3f, 1.0f));
	dirLight->SetSpecular(Vector4f(0.2f, 0.2f, 0.2f, 1.0f));
	dirLight->SetDirection(Vector3f(1.0f, -1.0f, 0.0f));

	Kz::Viewport& viewport = m_mainWindow->CreateViewport(0.0f, 0.0f, 1.0f, 1.0f);

	camera = &viewport.m_camera;

	Kz::Texture2D* stonewall = 
		m_sceneManager.GetTextureCreator().CreateTexture2D("..//Textures//stonewall.tga");

	Kz::Shader* animVertShader = m_sceneManager.GetRenderSystem().m_shaderManager.
		CreateShader(Kz::VERT_SHADER, "..//Sample//shaders//AnimGeomPassVert.cso");

	Kz::Shader* primitiveVertShader = m_sceneManager.GetRenderSystem().m_shaderManager.
		CreateShader(Kz::VERT_SHADER, "..//Sample//shaders//PrimitivesVert.cso");

	Kz::Shader* geometryPassFrag = m_sceneManager.GetRenderSystem().m_shaderManager.
		CreateShader(Kz::FRAG_SHADER, "..//Sample//shaders//GeometryPassFrag.cso");
	
	Kz::ShaderProgram* animProgram = m_sceneManager.GetRenderSystem().m_shaderManager.
		CreateShaderProgram({ animVertShader, geometryPassFrag });

	Kz::ShaderProgram* staticProgram = m_sceneManager.GetRenderSystem().m_shaderManager.
		CreateShaderProgram({ primitiveVertShader, geometryPassFrag });

	Kz::Entity* ember = m_sceneManager.GetEntityManager().
		CreateEntity("..\\Models\\Ember\\Ember.mesh.xml", *animProgram);

	ember->GetAnimationSystem().EnqueueAnimation("Attack1");
	ember->GetAnimationSystem().EnqueueAnimation("Attack1");
	ember->GetAnimationSystem().EnqueueAnimation("Attack1");
	ember->GetAnimationSystem().EnqueueAnimation("Attack1");
	ember->GetAnimationSystem().EnqueueAnimation("Attack1");
	ember->GetAnimationSystem().EnqueueAnimation("Idle");

	ember->SetCastingShadow(true);

	Kz::Entity* ember2 = m_sceneManager.GetEntityManager().
		CreateEntity("..\\Models\\Ember\\Ember.mesh.xml", *animProgram);

	ember2->GetAnimationSystem().EnqueueAnimation("Attack1");

	ember2->SetCastingShadow(true);

	Kz::SceneNode* sceneNode = m_sceneManager.GetScene().GetRootNode().CreateNode();
	characterNode = m_sceneManager.GetScene().GetRootNode().CreateNode();
	characterNode->SetPosition(Vector3f(0.0f, 0.0f, -0.0f));

	characterNode->AttachEntity(ember);
	
	viewport.m_camera.SetLookAt(characterNode->GetPosition());
	viewport.m_camera.SetPosition(characterNode->GetPosition() + Vector3f(0.0f, 10.0f, -10.0f));

	sceneNode = m_sceneManager.GetScene().GetRootNode().CreateNode();
	sceneNode->SetPosition(Vector3f(5.0f, 0.0f, -15.0f));

	sceneNode->AttachEntity(ember2);

	Kz::Material* groundMaterial = m_sceneManager.GetEntityManager().CreateMaterial(*staticProgram);
	groundMaterial->SetTexture("diffuseTexture", (Kz::Texture*) stonewall);
	Kz::Plane* plane = m_sceneManager.GetEntityManager().CreatePlane(50.0f, *groundMaterial);

	plane->SetCastingShadow(false);

	Kz::SpotLight* spotLight = m_sceneManager.CreateSpotLight();

	spotLight->SetAttenuation(Vector3f(0.f, 0.f, 0.000002f));
	spotLight->SetDiffuse(Vector4f(0.6f, 0.6f, 0.6f, 1.0f));
	spotLight->SetSpecular(Vector4f(0.8f, 0.8f, 0.8f, 1.0f));
	spotLight->SetDirection(Vector3f(-20.0f, 0.0f, 0.0f));
	spotLight->SetSpotCutoff(50.0f);
	spotLight->SetSpotExponent(10);

	sceneNode = m_sceneManager.GetScene().GetRootNode().CreateNode();

	sceneNode->SetPosition(Vector3f(0.0f, 12.0f, 16.0f));
	sceneNode->AttachLight(light);
	m_sceneManager.GetScene().AttachDirectionalLight(dirLight);

	sceneNode = m_sceneManager.GetScene().GetRootNode().CreateNode();

	sceneNode->SetPosition(Vector3f(16.0f, 12.0f, 0.0f));
	sceneNode->AttachLight(spotLight);

	sceneNode = m_sceneManager.GetScene().GetRootNode().CreateNode();

	sceneNode->SetPosition(Vector3f(0.0f, -10.0f, -100.0f));

	sceneNode = m_sceneManager.GetScene().GetRootNode().CreateNode();

	m_sceneManager.GetScene().GetRootNode().AttachEntity(plane);

	camera->SetLookAt(characterNode->GetPosition() + Vector3f(0.0f, 2.0f, 0.0f));
	camera->SetPosition(characterNode->GetPosition() + Vector3f(0.0f, 2.0f, -8.0f));

	AttackCommand* attack = new AttackCommand(ember);
	MoveForwardCommand* walk = new MoveForwardCommand(ember);
	StopMoveCommand* idle = new StopMoveCommand(ember);
	ExitCommand* exit = new ExitCommand(this);
	CameraMoveCommand* cameraMove 
		= new CameraMoveCommand(m_mainWindow, &viewport.m_camera, characterNode->GetPosition());
	LockMouseCommand* lockMouse = new LockMouseCommand(m_mainWindow);
	UnlockMouseCommand* unlockMouse = new UnlockMouseCommand(m_mainWindow);

	m_keyboardInputHandler.AddCommand(walk,		KEY_W,				Kz::STATE_DOWN);
	m_keyboardInputHandler.AddCommand(idle,		KEY_W,				Kz::STATE_RELEASED);
	m_keyboardInputHandler.AddCommand(attack,	KEY_SPACE,			Kz::STATE_PRESSED);
	m_keyboardInputHandler.AddCommand(exit,		KEY_ESCAPE,			Kz::STATE_PRESSED);

	m_mouseInputHandler.AddClickCommand(lockMouse, Kz::MOUSE_LB,	Kz::STATE_PRESSED);
	m_mouseInputHandler.AddClickCommand(unlockMouse, Kz::MOUSE_LB,	Kz::STATE_RELEASED);
	m_mouseInputHandler.AddMoveCommand(cameraMove);
}

void App::OnUpdate()
{

}

void App::OnExit()
{

}