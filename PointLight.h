#pragma once

#include "Graphics.h"
#include "LightSphere.h"

class PointLight
{
public:
	PointLight(Graphics& gfx);

	void SpawnImguiWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx) const noexcept;

private:
	struct PointLightCBuf
	{
		XMFLOAT3 pos;
		float padding = 0.0f;
	};

private:
	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	mutable LightSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};