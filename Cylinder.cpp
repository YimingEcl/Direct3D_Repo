#include "Cylinder.h"

Cylinder::Cylinder(Graphics& gfx, int longDiv)
	:
	longDiv(longDiv)
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};

	auto model = Prism::MakeTesselatedIndependentWithNormals<Vertex>(longDiv);

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
