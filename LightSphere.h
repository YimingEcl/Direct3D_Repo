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
	XMMATRIX GetTransformXM() const noexcept override;

private:
	XMFLOAT4 position = { 1.0f, 1.0f, 1.0f, 1.0f };
};