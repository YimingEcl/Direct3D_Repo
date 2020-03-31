#include "Ground.h"

Ground::Ground(Graphics& gfx)
{
	struct Vertex
	{
		XMFLOAT3 pos;
	};

	auto model = Plane::Make<Vertex>();

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

void Ground::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Plane"))
	{
		ImGui::Text("Size");
		ImGui::SliderFloat("Length", &scaling.x, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Width", &scaling.y, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Height", &scaling.z, 0.0f, 10.0f, "%.1f");
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

void Ground::Reset() noexcept
{
	scaling.x = 1.0f;
	scaling.y = 1.0f;
	scaling.z = 1.0f;
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
}

void Ground::Update(float dt) noexcept
{
}

DirectX::XMMATRIX Ground::GetTransformXM() const noexcept
{
	return XMMatrixScalingFromVector(XMLoadFloat4(&scaling)) *
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&rotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}