#include "SolidSphere.h"

SolidSphere::SolidSphere(Graphics& gfx, int latDiv, int longDiv)
	:
	latDiv(latDiv),
	longDiv(longDiv)
{
	struct Vertex
	{
		XMFLOAT3 pos;
	};

	auto model = Sphere::MakeTesselated<Vertex>(latDiv, longDiv);

	// bind to pipeline
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"SixColorVS.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"SixColorPS.cso"));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,0.0f,1.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
