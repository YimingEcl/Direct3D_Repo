#include "TransConstantBuffer.h"

TransConstantBuffer::TransConstantBuffer(Graphics& gfx, const Drawable& parent)
	:
	vcbuf(gfx),
	parent(parent)
{
}

void TransConstantBuffer::Bind(Graphics& gfx) noexcept
{
	vcbuf.Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	vcbuf.Bind(gfx);
}
