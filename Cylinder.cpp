#include "Cylinder.h"

Cylinder::Cylinder(Graphics& gfx, int longDiv)
{
	struct Vertex
	{
		XMFLOAT3 pos;
	};

	auto model = Prism::Make<Vertex>();

	// bind to pipeline
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"SixColorVS.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"SixColorPS.cso"));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	struct PSConstantBuffer
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} sixColor[6];
	};
	const PSConstantBuffer cb =
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
	AddBind(std::make_unique<PixelConstantBuffer<PSConstantBuffer>>(gfx, cb));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransConstantBuffer>(gfx, *this));
}

void Cylinder::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Cylinder"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("Pos_X", &position.x, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Pos_Y", &position.y, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Pos_Z", &position.z, -20.0f, 20.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Cylinder::Reset() noexcept
{
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Cylinder::Update(float dt) noexcept
{
}

XMMATRIX Cylinder::GetTransformXM() const noexcept
{
	return XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}
