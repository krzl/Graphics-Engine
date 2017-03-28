#pragma once

#include "Framework\Application.h"
#include "SceneNode.h"
#include "Camera.h"

class App : public Kz::Application
{
private:

	Kz::SceneNode* characterNode;
	Kz::Camera* camera;

public:

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnExit();
};