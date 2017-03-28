#include "Application.h"
#include "Utility\Timer.h"
#include "WinWindow.h"
#include "Math\Vector.h"

namespace Kz
{
	void Application::Init(int width, int height, std::string windowTitle)
	{
		m_mainWindow = new WinWindow(width, height, m_sceneManager.GetGraphicsDevice(), windowTitle);
		m_sceneManager.Init(width, height);
		m_keyboardInputHandler.Init(m_mainWindow);
		m_mouseInputHandler.Init(m_mainWindow);
		OnInit();
	}

	void Application::Update(double timeElapsed)
	{
		m_keyboardInputHandler.HandleInputs();
		m_mouseInputHandler.HandleInputs();
		OnUpdate();
		m_sceneManager.Update(timeElapsed);
		m_mainWindow->Update(m_sceneManager.GetRenderSystem());
		if (m_mainWindow->IsClosed())
			m_isRunning = false;
	}

	void Application::Run()
	{
		int i = 0;
		Timer timer;
		timer.Start();
		while (m_isRunning)
		{
			double timeElapsed = timer.GetDelta();
			if (timeElapsed >= MS_PER_UPDATE)
			{
				timer.Reset();
				Update(MS_PER_UPDATE);
			}
		}
	}

	void Application::Start()
	{
		m_isRunning = true;
		Run();
	}

	void Application::Exit()
	{
		OnExit();
		m_isRunning = false;
	}
}