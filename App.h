#pragma once

#include "Window.h"
#include "Timer.h"
#include "Camera.h"
#include "DrawableHeader.h"
#include "PointLight.h"
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
	Camera camera;
	PointLight light;
	std::unique_ptr<SolidSphere> sphere;
	std::unique_ptr<Box> cube;
	std::unique_ptr<Cylinder> cylinder;
	std::vector<std::unique_ptr<DancingCube>> dCubes;
	float speed_factor = 1.0f;
};