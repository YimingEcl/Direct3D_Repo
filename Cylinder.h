#pragma once

#include "BindableHeader.h"
#include "Drawable.h"
#include "Prism.h"

class Cylinder : public Drawable
{
public:
	Cylinder(Graphics& gfx, int longDiv);

	void SpawnImguiWindow() noexcept;
	void Reset() noexcept;
	void Update(float dt) noexcept override;
	XMMATRIX GetTransformXM() const noexcept override;

private:
	int longDiv;
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};