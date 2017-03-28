#include "Framework\Command.h"
#include "Engine\Camera.h"
#include "Framework\Application.h"
#include "Framework\MouseMoveCommand.h"

class AttackCommand : public Kz::Command
{
private:

	Kz::Entity* m_entity;

public:

	AttackCommand(Kz::Entity* entity);

	virtual void Execute();
};

class MoveForwardCommand : public Kz::Command
{
private:

	Kz::Entity* m_entity;

public:

	MoveForwardCommand(Kz::Entity* entity);

	virtual void Execute();
};

class StopMoveCommand : public Kz::Command
{
private:

	Kz::Entity* m_entity;

public:

	StopMoveCommand(Kz::Entity* entity);

	virtual void Execute();
};

class ExitCommand : public Kz::Command
{
private:

	Kz::Application* m_application;

public:

	ExitCommand(Kz::Application* application);

	virtual void Execute();
};

class LockMouseCommand : public Kz::Command
{
private:

	Kz::Window* m_window;

public:

	LockMouseCommand(Kz::Window* window);

	virtual void Execute();
};

class UnlockMouseCommand : public Kz::Command
{
private:

	Kz::Window* m_window;

public:

	UnlockMouseCommand(Kz::Window* window);

	virtual void Execute();
};

class CameraMoveCommand : public Kz::MouseMoveCommand
{
private:

	Kz::Camera* m_camera;
	const Vector3f& m_characterPosition;
	Kz::Window* m_window;

	float currentAngleX = 0.0f;
	float currentAngleY = 0.0f;

public:

	CameraMoveCommand(Kz::Window* window, Kz::Camera* camera, const Vector3f& characterPos);

	virtual void Execute();
};