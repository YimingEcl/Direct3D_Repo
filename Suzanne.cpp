#include "Suzanne.h"

Suzanne::Suzanne(Graphics& gfx)
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};

	Assimp::Importer imp;
	const auto pModel = imp.ReadFile("models\\suzanne.obj",
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	const auto pMesh = pModel->mMeshes[0];

	std::vector<Vertex> vertices;
	vertices.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices.push_back({
			{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z },
			*reinterpret_cast<XMFLOAT3*>(&pMesh->mNormals[i])
			});
	}

	std::vector<unsigned short> indices;
	indices.reserve(pMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		const auto& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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

void Suzanne::SpawnImguiWindow() noexcept
{
	if (ImGui::Begin("Suzanne"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("Pos_X", &position.x, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Pos_Y", &position.y, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Pos_Z", &position.z, -20.0f, 20.0f, "%.1f");
		ImGui::Text("Rotation");
		ImGui::SliderAngle("X", &rotation.x, -180.0f, 180.0f); //roll
		ImGui::SliderAngle("Y", &rotation.y, -180.0f, 180.0f); //pitch
		ImGui::SliderAngle("Z", &rotation.z, -180.0f, 180.0f); //yaw
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

void Suzanne::Reset() noexcept
{
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Suzanne::Update(Graphics& gfx, float dt) noexcept
{
	auto pConstPS = QueryBindable<objectCBuf>();
	assert(pConstPS != nullptr);
	pConstPS->Update(gfx, colorConst);
}

XMMATRIX Suzanne::GetTransformXM() const noexcept
{
	return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&rotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}
