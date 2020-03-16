#pragma once

#include "Exception.h"
#include "ExceptMacro.h"
#include "dxerr.h"
#include "DxgiInfoManager.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

namespace wrl = Microsoft::WRL;

class Graphics
{
	friend class Bindable;

public:
	class GraphicsException : public Exception
	{
	public:
		GraphicsException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:
		HRESULT hr;
		std::string info;
	};

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};

	class DeviceRemovedException : public GraphicsException
	{
		using GraphicsException::GraphicsException;
		
	public:
		const char* GetType() const noexcept override;

	private:
		std::string reason;
	};

public:
	Graphics (HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);

	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
 
private:
#ifndef NDEBUG
	DirectX::XMMATRIX projection;
	DxgiInfoManager infoManager;
#endif

	wrl::ComPtr<ID3D11Device> pDevice = nullptr;
	wrl::ComPtr<IDXGISwapChain> pSwap = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
};
