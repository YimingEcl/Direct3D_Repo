#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, WCHAR* path)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path, &pBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBlob() const noexcept
{
	return pBlob.Get();
}
