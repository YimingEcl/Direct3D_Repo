#include "PointLight.h"

PointLight::PointLight(Graphics& gfx)
	:
	mesh(gfx),
	cbuf(gfx)
{
}

void PointLight::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	pos = { 0.0f, 0.0f, 0.0f };
}

void PointLight::Draw(Graphics& gfx) noexcept(!IS_DEBUG)
{
	mesh.SetPos(pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) noexcept
{
	cbuf.Update(gfx, PointLightCBuff{ pos });
	cbuf.Bind(gfx);
}





