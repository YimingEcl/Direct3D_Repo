#pragma once

#include "BindableHeader.h"
#include "Drawable.h"
#include "Sphere.h"

class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, int latDiv, int longDiv);

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
	int latDiv = 12;
	int longDiv = 24;
	float radius = 1.0f;
	XMFLOAT4 scaling = XMFLOAT4(radius, radius, radius, 1.0f);
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};