#include "Graphics.h"
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace dx = DirectX;

// graphics exception
Graphics::GraphicsException::GraphicsException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::GraphicsException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::GraphicsException::GetType() const noexcept
{
	return "Graphics Exception";
}


HRESULT Graphics::GraphicsException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::GraphicsException::GetErrorString() const noexcept
{
	return DXGetErrorStringA(hr);
}

std::string Graphics::GraphicsException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::GraphicsException::GetErrorInfo() const noexcept
{
	return info;
}

// infomation only exception
Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

// device removed exception
const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

// graphics
Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// debug layer
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifndef NDEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking error
	HRESULT hr;

	// create device
	GFX_THROW_INFO(
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		)
	);

	// gain access to texture in swap chain
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

	// create depth stencil state
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsd, &pDSState));

	// bind depth stencil state to pipeline
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;

	D3D11_TEXTURE2D_DESC dtd = {};
	dtd.Width = 800.0f;
	dtd.Height = 600.0f;
	dtd.MipLevels = 1u;
	dtd.ArraySize = 1u;
	dtd.Format = DXGI_FORMAT_D32_FLOAT;
	dtd.SampleDesc.Count = 1u;
	dtd.SampleDesc.Quality = 0u;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO(pDevice->CreateTexture2D(&dtd, nullptr, &pDepthStencil));

	// create view of depth stencil texture
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvd, &pDSV));

	// bind depth stencil view to pipeline
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
}

void Graphics::EndFrame()
{
	HRESULT hr;

#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green ,blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::Draw(float angle, float x, float z)
{
	struct Vertex
	{
		float x;
		float y;
		float z;
	};

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	const std::vector<Vertex> vertices=
	{
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{-1.0f, 1.0f,-1.0f},
		{ 1.0f, 1.0f,-1.0f},
		{-1.0f,-1.0f, 1.0f},
		{ 1.0f,-1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f},
	};

	HRESULT hr;

	// create vertex buffer
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = UINT( sizeof( Vertex ) * vertices.size() );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = &indices;

	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	// bind index buffer to pipeline
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// create constant buffer for vertex shader
	struct ConstantBufferV
	{
		dx::XMMATRIX transform;
	};

	const ConstantBufferV cbv =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(angle) *
				dx::XMMatrixRotationX(angle) *
				dx::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
				dx::XMMatrixTranslation(x, 0.0f, z * 4.0f + 5.0f) *
				dx::XMMatrixPerspectiveFovLH(1.0f, 1.0f, 0.5f, 10.0f)
			)
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBufferV;

	D3D11_BUFFER_DESC cbdv = {};
	cbdv.ByteWidth = sizeof(cbv);
	cbdv.Usage = D3D11_USAGE_DYNAMIC;
	cbdv.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbdv.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbdv.MiscFlags = 0u;
	cbdv.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csdv = {};
	csdv.pSysMem = &cbv;

	GFX_THROW_INFO(pDevice->CreateBuffer(&cbdv, &csdv, &pConstantBufferV));

	// bind vertex shader constant buffer to pipeline
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBufferV.GetAddressOf());

	// create constant buffer for pixel shader
	struct ConstantBufferC
	{
		Color color[6] = {};
	};

	const ConstantBufferC cbc =
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBufferC;

	D3D11_BUFFER_DESC cbdc = {};
	cbdc.ByteWidth = sizeof(cbc);
	cbdc.Usage = D3D11_USAGE_DEFAULT;
	cbdc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbdc.CPUAccessFlags = 0u;
	cbdc.MiscFlags = 0u;
	cbdc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csdc = {};
	csdc.pSysMem = &cbc;

	GFX_THROW_INFO(pDevice->CreateBuffer(&cbdc, &csdc, &pConstantBufferC));

	// bind pixel shader constant buffer to pipeline
	pContext->PSSetConstantBuffers(0u, 1u, pConstantBufferC.GetAddressOf());

	// create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader to pipeline
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// bind vertex shader to pipeline
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// input (vertex) layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	// bind input layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// set primitive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

