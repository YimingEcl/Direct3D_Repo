#pragma once

#include "Window.h"
#include "Timer.h"
#include "DancingCube.h"
#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class App
{
public:
	App();
	~App();

	//master frame and message loop
	int Go();

private:
	void DoFrame();

private:
	ImguiManager imgui;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class DancingCube>> dCubes;
};