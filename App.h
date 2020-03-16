#pragma once

#include "Window.h"
#include "Timer.h"
#include "DancingCube.h"

class App
{
public:
	App();

	//master frame and message loop
	int Go();

private:
	void DoFrame();

private:
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class DancingCube>> dCubes;
};