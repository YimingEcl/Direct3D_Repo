#include "LightSphere.h"

LightSphere::LightSphere(Graphics& gfx)
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};

	auto model = Sphere::Make<Vertex>();

	// bind to pipeline
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"LightVS.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"LightPS.cso"));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	struct PSConstantBuffer
	{
		XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
		float padding = 1.0f;
	} colorConst;
	AddBind(std::make_unique<PixelConstantBuffer<PSConstantBuffer>>(gfx, colorConst, 0u));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT ,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransConstantBuffer>(gfx, *this));
}

void LightSphere::Update(float dt) noexcept
{
}

void LightSphere::SetPos(XMFLOAT3 pos) noexcept
{
	position = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
}

DirectX::XMMATRIX LightSphere::GetTransformXM() const noexcept
{
	return XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}
