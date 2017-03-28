#include "MouseInputHandler.h"
#include "Command.h"
#include "MouseMoveCommand.h"
#include "Engine\Window.h"

namespace Kz
{
	void MouseInputHandler::AddClickCommand(Command* command, MouseButton button, KeyState action)
	{
		m_mouseClickCommands.emplace_back(command, button, action);
	}

	void MouseInputHandler::AddMoveCommand(MouseMoveCommand* command)
	{
		m_mouseMoveCommands.push_back(command);
	}

	void MouseInputHandler::Init(Window* window)
	{
		m_window = window;
	}

	void MouseInputHandler::HandleInputs()
	{
		for (size_t i = 0; i < m_mouseClickCommands.size(); i++)
		{
			int currentKeyState = m_window->GetButtonState(m_mouseClickCommands[i].m_button);
			if (m_mouseClickCommands[i].m_action == currentKeyState)
			{
				m_mouseClickCommands[i].m_command->Execute();
			}
		}
		int dx, dy;
		m_window->GetMousePositionDiff(dx, dy);

		for (size_t i = 0; i < m_mouseMoveCommands.size(); i++)
		{
			m_mouseMoveCommands[i]->OnMouseMove(dx, dy);
		}
	}

	MouseInputHandler::~MouseInputHandler()
	{
		for (size_t i = 0; i < m_mouseClickCommands.size(); i++)
		{
			delete m_mouseClickCommands[i].m_command;
		}
		for (size_t i = 0; i < m_mouseMoveCommands.size(); i++)
		{
			delete m_mouseMoveCommands[i];
		}
	}

}