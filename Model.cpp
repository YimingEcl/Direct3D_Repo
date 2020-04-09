#include "Model.h"

Mesh::Mesh(Graphics& gfx, const aiMesh& mesh)
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};

	std::vector<Vertex> vertices;
	vertices.reserve(mesh.mNumVertices);
	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vertices.push_back({
			{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
			*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i])
			});
	}
	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
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

void Mesh::Update(Graphics& gfx, float dt) noexcept
{
}

XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&rotation)) * 
		XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}

Model::Model(Graphics& gfx, const std::string fileName)
{
	Assimp::Importer imp;
	const auto pModel = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	for (int i = 0; i < pModel->mNumMeshes; i++)
	{
		meshPtrs.push_back(std::make_shared<Mesh>(gfx, *pModel->mMeshes[i]));
	}
}

void Model::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	for (int i = 0; i < meshPtrs.size(); i++)
	{
		auto mesh = meshPtrs[i];
		mesh->Draw(gfx);
	}
}
