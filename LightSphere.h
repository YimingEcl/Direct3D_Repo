#pragma once

#include "BindableHeader.h"
#include "Drawable.h"
#include "Sphere.h"

class LightSphere : public Drawable
{
public:
	LightSphere(Graphics& gfx);

	void Update(Graphics& gfx, float dt) noexcept;
	void SetPos(XMFLOAT3 pos) noexcept;
	XMMATRIX GetTransformXM() const noexcept override;

private:
	float radius = 0.3f;
	XMFLOAT4 scaling = XMFLOAT4(radius, radius, radius, 1.0f);
	XMFLOAT4 position = { 1.0f, 1.0f, 1.0f, 1.0f };
};