#include "App.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int){

	App app;
	app.Init(800, 600, "Test");
	app.Start();
}