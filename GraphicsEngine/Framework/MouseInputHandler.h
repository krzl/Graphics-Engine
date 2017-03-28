#pragma once

#include <vector>

namespace Kz
{
	class Command;
	class MouseMoveCommand;
	class Window;

	enum MouseButton;
	enum KeyState;

	class MouseInputHandler
	{
		friend class Application;

	private:

		struct MouseClickCommandBinding
		{
			Command*								m_command;
			MouseButton								m_button;
			KeyState								m_action;

			MouseClickCommandBinding(Command* command, MouseButton button, KeyState action) :
				m_command(command),
				m_button(button),
				m_action(action)
			{}
		};

		Window*										m_window;
		std::vector<MouseClickCommandBinding>		m_mouseClickCommands;
		std::vector<MouseMoveCommand*>				m_mouseMoveCommands;

		MouseInputHandler() {}
		~MouseInputHandler();

	public:

		void										AddClickCommand(Command* command, MouseButton button, KeyState action);
		void										AddMoveCommand(MouseMoveCommand* command);
		void										Init(Window* window);
		void										HandleInputs();
	};
}