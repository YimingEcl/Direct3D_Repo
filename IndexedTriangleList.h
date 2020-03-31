#pragma once

#include <array>
#include <vector>
#include "Math.h"

using namespace DirectX;

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> vertices_in, std::vector<unsigned short> indices_in)
		:
		vertices(std::move(vertices_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& v : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(
				&v.pos,
				DirectX::XMVector3Transform(pos, matrix)
			);
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};