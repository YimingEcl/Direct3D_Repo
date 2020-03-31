#pragma once

#include "BindableHeader.h"
#include "Drawable.h"
#include "Sphere.h"

class LightSphere : public Drawable
{
public:
	LightSphere(Graphics& gfx);

	void Update(float dt) noexcept;
	void SetPos(XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};