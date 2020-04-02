#include "SolidSphere.h"

SolidSphere::SolidSphere(Graphics& gfx, int latDiv, int longDiv)
	:
	latDiv(latDiv),
	longDiv(longDiv)
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};

	auto model = Sphere::MakeTesselated<Vertex>(latDiv, longDiv);

	// bind to pipeline
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	struct PSObjectCBuf
	{
		alignas(16) XMFLOAT3 color = { 1.0f, 0.0f, 0.0f };
		float specularIntensity = 0.6f;
		float specularPower = 100.0f;
		float padding[2] = { 0.0f, 0.0f };
	} colorConst;
	AddBind(std::make_unique<PixelConstantBuffer<PSObjectCBuf>>(gfx, colorConst, 1u));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT ,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransConstantBuffer>(gfx, *this));
}

void SolidSphere::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Sphere"))
	{
		ImGui::Text("Basic");
		ImGui::SliderFloat("Radius", &radius, 0.5f, 20.0f, "%.1f");
		ImGui::Text("Position");
		ImGui::SliderFloat("Pos_X", &position.x, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Pos_Y", &position.y, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Pos_Z", &position.z, -20.0f, 20.0f, "%.1f");
		ImGui::Text("Rotation");
		ImGui::SliderAngle("X", &rotation.x, -180.0f, 180.0f); //roll
		ImGui::SliderAngle("Y", &rotation.y, -180.0f, 180.0f); //pitch
		ImGui::SliderAngle("Z", &rotation.z, -180.0f, 180.0f); //yaw
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void SolidSphere::Reset() noexcept
{
	radius = 1.0f;
	scaling = XMFLOAT4(radius, radius, radius, 1.0f);
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void SolidSphere::Update(float dt) noexcept
{
	scaling = XMFLOAT4(radius, radius, radius, 1.0f);
}

XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return XMMatrixScalingFromVector(XMLoadFloat4(&scaling)) *
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&rotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}
