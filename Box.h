#pragma once

#include "Drawable.h"
#include "BindableHeader.h"

class Box : public Drawable
{
public:
	Box(Graphics& gfx);

	void SpawnImguiWindow() noexcept;
	void Reset() noexcept;
	void Update(float dt) noexcept override;
	XMMATRIX GetTransformXM() const noexcept override;

private:
	XMFLOAT4 scaling = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};