#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, WCHAR* path);
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetBlob() const noexcept;

protected:
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3DBlob> pBlob;
};