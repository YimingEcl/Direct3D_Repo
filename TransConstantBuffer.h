#pragma once

#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransConstantBuffer: public Bindable
{
public:
	TransConstantBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;

private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};