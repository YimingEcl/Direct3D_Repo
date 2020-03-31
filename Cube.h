#pragma once

#include "IndexedTriangleList.h"

class Cube
{
public:
	template<class T>
	static IndexedTriangleList<T> Make()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<T> vertices(8);
		vertices[0].pos = { -side, -side, -side };
		vertices[1].pos = {  side, -side, -side };
		vertices[2].pos = { -side,  side, -side };
		vertices[3].pos = {  side,  side, -side };
		vertices[4].pos = { -side, -side,  side };
		vertices[5].pos = {  side, -side,  side };
		vertices[6].pos = { -side,  side,  side };
		vertices[7].pos = {  side,  side,  side };

		return{
			std::move(vertices),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}
};