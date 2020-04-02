#pragma once

#include "IndexedTriangleList.h"

class Prism
{
public:
	template<class T>
	static IndexedTriangleList<T> MakeTesselated(int longDiv)
	{
		assert(longDiv >= 3);

		const auto base = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto offset = XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		// near center
		std::vector<T> vertices;
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		const auto iCenterNear = (unsigned short)(vertices.size() - 1);

		// far center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		const auto iCenterFar = (unsigned short)(vertices.size() - 1);

		// base vertices
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			// near base
			{
				vertices.emplace_back();
				auto v = XMVector3Transform(base, XMMatrixRotationZ(longitudeAngle * iLong));
				XMStoreFloat3(&vertices.back().pos, v);
			}

			// far base
			{
				vertices.emplace_back();
				auto v = XMVector3Transform(base, XMMatrixRotationZ(longitudeAngle * iLong));
				v = XMVectorAdd(v, offset);
				XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		// side indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + 2);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back(i + 1 + 2);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back((i + 3) % mod + 2);
			indices.push_back(i + 1 + 2);
		}

		// base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + 2);
			indices.push_back(iCenterNear);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back(iCenterFar);
			indices.push_back(i + 1 + 2);
			indices.push_back((i + 3) % mod + 2);
		}

		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentWithNormals(int longDiv)
	{
		assert(longDiv >= 3);

		const auto base = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto offset = XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;

		// near center
		const auto iCenterNear = (unsigned short)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		vertices.back().normal = { 0.0f,0.0f,-1.0f };
		// near base vertices
		const auto iBaseNear = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = XMVector3Transform(base, XMMatrixRotationZ(longitudeAngle * iLong));
			XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().normal = { 0.0f,0.0f,-1.0f };
		}

		// far center
		const auto iCenterFar = (unsigned short)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		vertices.back().normal = { 0.0f,0.0f,1.0f };
		// far base vertices
		const auto iBaseFar = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = XMVector3Transform(base, XMMatrixRotationZ(longitudeAngle * iLong));
			v = XMVectorAdd(v, offset);
			XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().normal = { 0.0f,0.0f,1.0f };
		}

		// fusilage vertices
		const auto iFusilage = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			// near base
			{
				vertices.emplace_back();
				auto v = XMVector3Transform(base, XMMatrixRotationZ(longitudeAngle * iLong));
				XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().normal = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
			}

			// far base
			{
				vertices.emplace_back();
				auto v = XMVector3Transform(base, XMMatrixRotationZ(longitudeAngle * iLong));
				v = XMVectorAdd(v, offset);
				XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().normal = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
			}
		}

		std::vector<unsigned short> indices;

		// near base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong;
			const auto mod = longDiv;
			// near
			indices.push_back(i + iBaseNear);
			indices.push_back(iCenterNear);
			indices.push_back((i + 1) % mod + iBaseNear);
		}

		// far base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong;
			const auto mod = longDiv;
			// far
			indices.push_back(iCenterFar);
			indices.push_back(i + iBaseFar);
			indices.push_back((i + 1) % mod + iBaseFar);
		}

		// fusilage indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + iFusilage);
			indices.push_back((i + 2) % mod + iFusilage);
			indices.push_back(i + 1 + iFusilage);
			indices.push_back((i + 2) % mod + iFusilage);
			indices.push_back((i + 3) % mod + iFusilage);
			indices.push_back(i + 1 + iFusilage);
		}

		return { std::move(vertices),std::move(indices) };
	}

	template<class T>
	static IndexedTriangleList<T> Make()
	{
		return MakeTesselated<T>(24);
	}
};