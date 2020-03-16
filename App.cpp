#include "App.h"

App::App()
	:
	wnd(800, 600, L"Main Window")
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
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().Draw(
		-timer.Peek(),
		0.0f, 0.0f
	);
	wnd.Gfx().Draw(
		timer.Peek(),
		wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-wnd.mouse.GetPosY() / 300.0f + 1.0f
	);
	wnd.Gfx().EndFrame();
}
