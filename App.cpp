#include "App.h"

App::App()
	:
	wnd(1280, 720, L"Main Window"),
	light(wnd.Gfx())
{
	//std::mt19937 rng(std::random_device{}());
	//std::uniform_real_distribution<float> adist(0.0f, PI * 2.0f);
	//std::uniform_real_distribution<float> ddist(0.0f, PI * 2.0f);
	//std::uniform_real_distribution<float> odist(0.0f, PI * 0.3f);
	//std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	//std::uniform_real_distribution<float> bdist(0.4f, 3.0f);
	//std::uniform_real_distribution<float> cdist(0.0f, 1.0f);

	// dancing cube
	//for (auto i = 0; i < 80; i++)
	//{
	//	dCubes.push_back(std::make_unique<DancingCube>(
	//		wnd.Gfx(), rng, adist, ddist,
	//		odist, rdist, bdist, cdist
	//		));
	//}

	// initialize object
	//cube = std::make_unique<Box>(wnd.Gfx());
	//suzanne = std::make_unique<Suzanne>(wnd.Gfx());
	//sphere = std::make_unique<SolidSphere>(wnd.Gfx(), 15, 15);
	//cylinder = std::make_unique<Cylinder>(wnd.Gfx(), 18);
	model = std::make_unique<Model>(wnd.Gfx(), "models\\nanosuit.obj");

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 1.0f, 100.0f));
}

App::~App()
{
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessage())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeignFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(camera.GetMatrix());
	light.Bind(wnd.Gfx(), camera.GetMatrix());

	// dancing cube
	//for (auto& b : dCubes)
	//{
	//	b->Update(wnd.Gfx(), wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	//	b->Draw(wnd.Gfx());
	//}

	// draw object
	//cube->Draw(wnd.Gfx());
	//sphere->Draw(wnd.Gfx());
	//cylinder->Draw(wnd.Gfx());
	//suzanne->Draw(wnd.Gfx());
	model->Draw(wnd.Gfx());

	light.Draw(wnd.Gfx()); // light must be drawn at the end.

	if (ImGui::Begin("Speed Control"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("State: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSE" : "RUNNING");
	}
	ImGui::End();

	// update
	//sphere->Update(wnd.Gfx(), dt);
	//cylinder->Update(wnd.Gfx(), dt);
	//suzanne->Update(wnd.Gfx(), dt);

	// control
	//cube->SpawnImguiWindow();
	//sphere->SpawnImguiWindow();
	//cylinder->SpawnImguiWindow();
	//suzanne->SpawnImguiWindow();
	model->SpawnImguiWindow();

	camera.SpawnImguiWindow();
	light.SpawnImguiWindow();

	// present 
	wnd.Gfx().EndFrame();
}
