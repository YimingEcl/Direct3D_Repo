#include "DancingCube.h"

DancingCube::DancingCube(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	std::uniform_real_distribution<float>& cdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),   //local x rotation
	dpitch(ddist(rng)),  //local y rotation
	dyaw(ddist(rng)),    //local z rotation
	dphi(odist(rng)),    //world x rotation
	dtheta(odist(rng)),  //world y rotation
	dchi(odist(rng)),    //world z rotation
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng)),
	materialColor({cdist(rng), cdist(rng), cdist(rng)})
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

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT ,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransConstantBuffer>(gfx, *this));

	struct PSObjectCBuf
	{
		alignas(16) XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2] = {0.0f, 0.0f};
	} colorConst;

	colorConst.color = materialColor;
	AddBind(std::make_unique<PixelConstantBuffer<PSObjectCBuf>>(gfx, colorConst, 1u));

	XMStoreFloat3x3(&mt, XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

void DancingCube::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX DancingCube::GetTransformXM() const noexcept
{
	return XMLoadFloat3x3(&mt) *
		XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		XMMatrixTranslation(r, 0.0f, 0.0f) *
		XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
