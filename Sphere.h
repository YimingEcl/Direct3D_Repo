#pragma once

#include "IndexedTriangleList.h"

class Sphere
{
public:
	template<class T>
	static IndexedTriangleList<T> MakeTesselated(int latDiv, int longDiv)
	{
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		constexpr float radius = 0.5f;
		const float latAngle = PI / latDiv;
		const float longAngle = 2.0f * PI / longDiv;
		const XMVECTOR base = XMVectorSet(0.0f, radius, 0.0f, 0.0f);
		std::vector<T> vertices;

		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			XMVECTOR latBase = XMVector3Transform(base, XMMatrixRotationZ(iLat * latAngle));
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				XMVECTOR fin_pos = XMVector3Transform(latBase, XMMatrixRotationY(iLong * longAngle));
				XMStoreFloat3(&vertices.back().pos, fin_pos);
				XMStoreFloat3(&vertices.back().normal, fin_pos);
			}
		}

		const auto iNorthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		XMStoreFloat3(&vertices.back().pos, base);
		XMStoreFloat3(&vertices.back().normal, base);
		const auto iSouthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		XMStoreFloat3(&vertices.back().pos, XMVectorNegate(base));
		XMStoreFloat3(&vertices.back().normal, XMVectorNegate(base));

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

		return { std::move(vertices),std::move(indices) };
	}

	template<class T>
	static IndexedTriangleList<T> Make()
	{
		return MakeTesselated<T>(12, 24);
	}
};