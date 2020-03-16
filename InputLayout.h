#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pBlob);
	void Bind(Graphics& gfx) noexcept override;

protected:
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
};