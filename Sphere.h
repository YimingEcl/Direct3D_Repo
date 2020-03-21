#pragma once

#include "Drawable.h"
#include "BindableHeader.h"
#include "Math.h"

class Sphere : public Drawable
{
public:
	Sphere(Graphics& gfx, int latDiv, int longDiv);

	void SpawnImguiWindow() noexcept;
	void Reset() noexcept;
	void Update(float dt) noexcept override;
	XMMATRIX GetTransformXM() const noexcept override;

private:
	int latDiv = 12;
	int longDiv = 24;
	float radius = 1.0f;
	XMFLOAT4 scaling = XMFLOAT4(radius, radius, radius, 1.0f);
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};