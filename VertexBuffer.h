#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
		:
		stride(sizeof(T))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = UINT(sizeof(T) * vertices.size());
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		GFX_THROW_INFO(GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}

	void Bind(Graphics& gfx) noexcept override;

protected:
	UINT stride;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
};
