#include "Bindable.h"

ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
{
	return gfx.pDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
{
	return gfx.pContext.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept(IS_DEBUG)
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error("Unable to access info manager in non-debug mode.");
#endif
}
