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
	void Update(Graphics& gfx, float dt) noexcept override;
	XMMATRIX GetTransformXM() const noexcept override;

private:
	struct PSObjectCBuf
	{
		alignas(16) XMFLOAT3 color = { 1.0f, 0.0f, 0.0f };
		float specularIntensity = 0.6f;
		float specularPower = 100.0f;
		float padding[2] = { 0.0f, 0.0f };
	} colorConst;

	using objectCBuf = PixelConstantBuffer<PSObjectCBuf>;

private:
	int longDiv = 24;
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 rotation = XMFLOAT4(PI / 2, 0.0f, 0.0f, 1.0f);
};