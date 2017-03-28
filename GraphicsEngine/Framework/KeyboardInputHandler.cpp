#include "KeyboardInputHandler.h"
#include "Command.h"
#include "Engine\Window.h"

namespace Kz
{
	void KeyboardInputHandler::AddCommand(Command* command, int key, KeyState action)
	{
		m_commands.emplace_back(command, key, action);
	}

	void KeyboardInputHandler::Init(Window* window)
	{
		m_window = window;
	}

	void KeyboardInputHandler::HandleInputs()
	{
		for (size_t i = 0; i < m_commands.size(); i++)
		{
			int currentKeyState = m_window->GetKeyState(m_commands[i].m_key);
			if (m_commands[i].m_action == currentKeyState)
			{
				m_commands[i].m_command->Execute();
			}
		}
	}

	KeyboardInputHandler::~KeyboardInputHandler()
	{
		for (size_t i = 0; i < m_commands.size(); i++)
		{
			delete m_commands[i].m_command;
		}
	}
}