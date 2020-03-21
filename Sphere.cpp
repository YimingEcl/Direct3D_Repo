#include "Sphere.h"

Sphere::Sphere(Graphics& gfx, int latDiv, int longDiv)
	:
	latDiv(latDiv),
	longDiv(longDiv)
{
	struct Vertex
	{
		XMFLOAT3 pos;
	};

	// vertices data
	const float latAngle = PI / latDiv;
	const float longAngle = 2.0f * PI / longDiv;
	const XMVECTOR base = XMVectorSet(0.0f, radius, 0.0f, 0.0f);
	std::vector<Vertex> vertices;

	for (int iLat = 1; iLat < latDiv; iLat++)
	{
		XMVECTOR latBase = XMVector3Transform(base, XMMatrixRotationZ(iLat * latAngle));
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			XMVECTOR fin_pos = XMVector3Transform(latBase, XMMatrixRotationY(iLong * longAngle));
			XMStoreFloat3(&vertices.back().pos, fin_pos);
		}
	}

	const auto iNorthPole = (unsigned short)vertices.size();
	vertices.emplace_back();
	XMStoreFloat3(&vertices.back().pos, base);
	const auto iSouthPole = (unsigned short)vertices.size();
	vertices.emplace_back();
	XMStoreFloat3(&vertices.back().pos, XMVectorNegate(base));

	const int temp_lat = latDiv;
	const int temp_long = longDiv;

	// indexed data
	const auto calcIdx = [temp_lat, temp_long](unsigned short iLat, unsigned short iLong)
	{ return iLat * temp_long + iLong; };
	std::vector<unsigned short> indices;
	for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
	{
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			indices.push_back(calcIdx(iLat, iLong));
			indices.push_back(calcIdx(iLat + 1, iLong));
			indices.push_back(calcIdx(iLat, iLong + 1));
			indices.push_back(calcIdx(iLat, iLong + 1));
			indices.push_back(calcIdx(iLat + 1, iLong));
			indices.push_back(calcIdx(iLat + 1, iLong + 1));
		}
		// wrap band
		indices.push_back(calcIdx(iLat, longDiv - 1));
		indices.push_back(calcIdx(iLat + 1, longDiv - 1));
		indices.push_back(calcIdx(iLat, 0));
		indices.push_back(calcIdx(iLat, 0));
		indices.push_back(calcIdx(iLat + 1, longDiv - 1));
		indices.push_back(calcIdx(iLat + 1, 0));
	}

	for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
	{
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, iLong));
		indices.push_back(calcIdx(0, iLong + 1));
		// south
		indices.push_back(calcIdx(latDiv - 2, iLong + 1));
		indices.push_back(calcIdx(latDiv - 2, iLong));
		indices.push_back(iSouthPole);
	}
	// special triangles
	// north
	indices.push_back(iNorthPole);
	indices.push_back(calcIdx(0, longDiv - 1));
	indices.push_back(calcIdx(0, 0));
	// south
	indices.push_back(calcIdx(latDiv - 2, 0));
	indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
	indices.push_back(iSouthPole);

	// bind to pipeline
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto pvsbc = pvs->GetBlob();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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

void Sphere::SpawnImguiWindow() noexcept
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

void Sphere::Reset() noexcept
{
	radius = 1.0f;
	scaling = XMFLOAT4(radius, radius, radius, 1.0f);
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Sphere::Update(float dt) noexcept
{
	scaling = XMFLOAT4(radius, radius, radius, 1.0f);
}

XMMATRIX Sphere::GetTransformXM() const noexcept
{
	return XMMatrixScalingFromVector(XMLoadFloat4(&scaling)) *
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(&rotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat4(&position));
}
