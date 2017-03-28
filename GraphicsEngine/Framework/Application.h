#pragma once

#include <iostream>

#include "Engine/SceneManager.h"
#include "WinWindow.h"
#include "KeyboardInputHandler.h"
#include "MouseInputHandler.h"

#define TARGET_FRAMERATE 60.0
#define MS_PER_UPDATE (1.0/TARGET_FRAMERATE)

namespace Kz
{
	class WinWindow;

	class Application
	{
	private:

		bool						m_isRunning = false;

		void						Run();
		void						Update(double timeElapsed);

	protected:

		WinWindow*					m_mainWindow;
		SceneManager				m_sceneManager;

		virtual void				OnUpdate() = 0;
		virtual void				OnExit() = 0;
		virtual void				OnInit() = 0;

	public:

		MouseInputHandler			m_mouseInputHandler;
		KeyboardInputHandler		m_keyboardInputHandler;

		void						Init(int width, int height, std::string windowTitle);
		void						Start();
		void						Exit();
	};
}