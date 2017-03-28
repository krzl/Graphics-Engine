#include "Commands.h"
#include "Engine\Entity.h"
#include "Engine\AnimationSystem.h"

#include "Utility\Logger.h"

AttackCommand::AttackCommand(Kz::Entity* entity)
{
	m_entity = entity;
}

void AttackCommand::Execute()
{
	m_entity->GetAnimationSystem().RunAnimation("Attack1");
}

MoveForwardCommand::MoveForwardCommand(Kz::Entity* entity)
{
	m_entity = entity;
}

void MoveForwardCommand::Execute()
{
	m_entity->GetAnimationSystem().RunAnimation("Walk");
}

StopMoveCommand::StopMoveCommand(Kz::Entity* entity)
{
	m_entity = entity;
}

void StopMoveCommand::Execute()
{
	m_entity->GetAnimationSystem().RunAnimation("Idle");
}

ExitCommand::ExitCommand(Kz::Application* framework)
{
	m_application = framework;
}

void ExitCommand::Execute()
{
	m_application->Exit();
}

LockMouseCommand::LockMouseCommand(Kz::Window* window)
{
	m_window = window;
}

void LockMouseCommand::Execute()
{
	m_window->LockMouse();
}

UnlockMouseCommand::UnlockMouseCommand(Kz::Window* window)
{
	m_window = window;
}

void UnlockMouseCommand::Execute()
{
	m_window->UnlockMouse();
}

CameraMoveCommand::CameraMoveCommand(Kz::Window* window, Kz::Camera* camera, const Vector3f& characterPos) :
	m_characterPosition(characterPos)
{
	m_camera = camera;
	m_window = window;
}

void CameraMoveCommand::Execute()
{
	if (!m_window->IsMouseLocked())
	{
		m_camera->SetLookAt(m_characterPosition + Vector3f(0.0f, 2.0f, 0.0f));
		currentAngleY += m_dx;
		currentAngleX += m_dy / 4.0f;

		if (currentAngleX < 0.0f)
			currentAngleX = 0.0f;
		if (currentAngleX > 90.0f)
			currentAngleX = 90.0f;

		float currAngleXRad = (currentAngleX * 3.1415f) / 180.0f;

		float x = (cosf(currAngleXRad) * sinf(currentAngleY / 320.f)) * 8.0f;
		float z = (cosf(currAngleXRad) * cosf(currentAngleY / 320.f)) * -8.0f;
		float y = sinf(currAngleXRad) * 8.0f + 2.0f;

		m_camera->SetPosition(m_characterPosition + Vector3f(x, y, z));
	}
}