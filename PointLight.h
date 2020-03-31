#pragma once

#include "Graphics.h"
#include "Sphere.h"

class PointLight
{
public:
	PointLight(Graphics& gfx);

	void SpawnImguiWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx) noexcept;

private:
	struct PointLightCBuff
	{
		XMFLOAT3 position;
		float padding;
	};

private:
	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	mutable Sphere mesh;
	mutable PixelConstantBuffer<PointLightCBuff> cbuf;
};