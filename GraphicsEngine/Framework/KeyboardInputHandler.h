#pragma once

#include <vector>

namespace Kz
{
	class Command;
	class Window;
	class Framework;

	enum KeyState;

	class KeyboardInputHandler
	{
		friend class Application;

	private:

		struct KeyboardCommandBinding
		{
			Command*								m_command;
			int										m_key;
			KeyState								m_action;

			KeyboardCommandBinding(Command* command, int key, KeyState action) :
				m_command(command),
				m_key(key),
				m_action(action)
			{}
		};

		Window*										m_window;
		std::vector<KeyboardCommandBinding>			m_commands;

		~KeyboardInputHandler();

	public:

		void										AddCommand(Command* command, int key, KeyState action);
		void										Init(Window* window);
		void										HandleInputs();
	};
}