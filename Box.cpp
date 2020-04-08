#include "Box.h"

Box::Box(Graphics& gfx)
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};
	auto model = Cube::MakeIndependent<Vertex>();
	model.SetNormalsIndependentFlat();

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	AddBind(std::make_unique<objectCBuf>(gfx, colorConst, 1u));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT ,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransConstantBuffer>(gfx, *this));
}

void Box::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Cube"))
	{
		ImGui::Text("Scaling");
		ImGui::SliderFloat("S_X", &scaling.x, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("S_Y", &scaling.y, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("S_Z", &scaling.z, 0.0f, 10.0f, "%.1f");
		ImGui::Text("Position");
		ImGui::SliderFloat("P_X", &position.x, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("P_Y", &position.y, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("P_Z", &position.z, -20.0f, 20.0f, "%.1f");
		ImGui::Text("Rotation");
		ImGui::SliderAngle("R_X", &rotation.x, -180.0f, 180.0f); //roll
		ImGui::SliderAngle("R_Y", &rotation.y, -180.0f, 180.0f); //pitch
		ImGui::SliderAngle("R_Z", &rotation.z, -180.0f, 180.0f); //yaw
		ImGui::Text("Specular/Color");
		ImGui::SliderFloat("Intensity", &colorConst.specularIntensity, 0.01f, 20.0f, "%.2f", 2);
		ImGui::SliderFloat("Power", &colorConst.specularPower, 0.01f, 200.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &colorConst.color.x);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Box::Reset() noexcept
{
	colorConst.color = { 1.0f, 0.0f, 0.0f };
	colorConst.specularIntensity = 0.6f;
	colorConst.specularPower = 100.0f;

	scaling = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Box::Update(Graphics& gfx, float dt) noexcept
{
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return XMMatrixScalingFromVector(XMLoadFloat4(&scaling)) *
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&rotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}
