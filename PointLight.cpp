#include "PointLight.h"

PointLight::PointLight(Graphics& gfx)
	:
	mesh(gfx),
	cbuf(gfx)
{
	Reset();
}

void PointLight::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.pos.x, -15.0f, 15.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.pos.y, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.pos.z, -40.0f, 40.0f, "%.1f");
		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.01f, 5.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);
		ImGui::Text("Attenuation");
		ImGui::SliderFloat("Linear", &cbData.linear_attenuation, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &cbData.quadradic_attenuation, 0.0000001f, 10.0f, "%.7f", 10);
		ImGui::SliderFloat("Constant", &cbData.constant_attenuation, 0.05f, 10.0f, "%.2f", 4);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}


void PointLight::Reset() noexcept
{
	cbData = {
	{ -3.0f,2.0f,-5.0f },
	{ 0.05f,0.05f,0.05f },
	{ 1.0f,1.0f,1.0f },
	1.0f,
	0.045f,
	0.0075f,
	1.0f,
	};
}

void PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	mesh.SetPos(cbData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, FXMMATRIX cameraView) const noexcept
{
	const auto pos = XMLoadFloat3(&cbData.pos);
	auto updateData = cbData;
	XMStoreFloat3(&updateData.pos, XMVector3Transform(pos, cameraView));
	cbuf.Update(gfx, updateData);
	cbuf.Bind(gfx);
}