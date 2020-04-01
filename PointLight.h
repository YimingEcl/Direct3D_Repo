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
		alignas(16) XMFLOAT3 pos;
		alignas(16) XMFLOAT3 ambient;
		alignas(16) XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float linear_attenuation;
		float quadradic_attenuation;
		float constant_attenuation;
	};

private:
	PointLightCBuf cbData;
	mutable LightSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};