#pragma once

#include "BindableHeader.h"
#include "Drawable.h"
#include "Cube.h"

class DancingCube : public Drawable
{
public:
	DancingCube(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		std::uniform_real_distribution<float>& cdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	// position parameters
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	// transfomation parameters
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	// model color
	XMFLOAT3 materialColor;

	// model transform
	XMFLOAT3X3 mt;
};