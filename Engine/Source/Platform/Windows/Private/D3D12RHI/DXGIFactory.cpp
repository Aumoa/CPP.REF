// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIFactory.h"
#include "DXGIAdapter.h"
#include "DXGISwapChain.h"
#include "D3D12Device.h"
#include "D3D12CommandQueue.h"
#include "DWriteFontCollection.h"
#include "DWriteTextFormat.h"
#include "DWriteTextLayout.h"
#include "WindowsApplication.h"

SDXGIFactory::SDXGIFactory(SWindowsApplication* App) : Super()
	, App(App)
{
	UINT flags = 0;
#if defined(_DEBUG)
	flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(CreateDXGIFactory2(flags, IID_PPV_ARGS(&_factory)));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory3), static_cast<IUnknown**>(&_writeFactory)));
}

IApplicationInterface* SDXGIFactory::GetApplication()
{
	return App;
}

IRHIAdapter* SDXGIFactory::GetAdapter(int32 index)
{
	if (_cachedAdapters.size() > index && _cachedAdapters[index] != nullptr)
	{
		return _cachedAdapters[index];
	}

	ComPtr<IDXGIAdapter1> adapter;
	HRESULT hr = _factory->EnumAdapters1((UINT)index, &adapter);
	if (hr == DXGI_ERROR_NOT_FOUND)
	{
		return nullptr;
	}
	else
	{
		HR(("_factory->EnumAdapters1", hr));
	}

	auto* instance = NewObject<SDXGIAdapter>(this, std::move(adapter));
	if (_cachedAdapters.size() <= (size_t)index)
	{
		_cachedAdapters.resize((size_t)index + 1);
	}
	_cachedAdapters[index] = instance;
	return instance;
}

IRHIDevice* SDXGIFactory::CreateDevice(IRHIAdapter* adapter)
{
	auto instance = Cast<SDXGIAdapter>(adapter);
	IDXGIAdapter1* actualAdapter = instance->GetAdapter();

#if defined(_DEBUG)
	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}
#endif

	ComPtr<ID3D12Device> device;
	HR(D3D12CreateDevice(actualAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));

	return NewObject<SD3D12Device>(this, std::move(device));
}

IRHISwapChain* SDXGIFactory::CreateSwapChain(IRHIDevice* device)
{
	HWND wHandle = App->GetWindowHandle();

	DXGI_SWAP_CHAIN_DESC1 desc1 =
	{
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED
	};

	auto primaryQueue = Cast<SD3D12CommandQueue>(device->GetImmediateContext());

	ComPtr<IDXGISwapChain1> swapChain1;
	HR(_factory->CreateSwapChainForHwnd(primaryQueue->Get<ID3D12CommandQueue>(), wHandle, &desc1, nullptr, nullptr, &swapChain1));

	ComPtr<IDXGISwapChain4> swapChain4;
	HR(swapChain1.As<IDXGISwapChain4>(&swapChain4));

	return NewObject<SDXGISwapChain>(this, Cast<SD3D12Device>(device), std::move(swapChain4));
}

IRHIFontCollection* SDXGIFactory::CreateFontCollection(const std::filesystem::path& path)
{
	ComPtr<IDWriteFontFile> fileReference;
	HR(_writeFactory->CreateFontFileReference(path.wstring().c_str(), nullptr, &fileReference));

	ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	HR(_writeFactory->CreateFontSetBuilder(&fontSetBuilder));
	HR(fontSetBuilder->AddFontFile(fileReference.Get()));

	ComPtr<IDWriteFontSet> fontSet;
	HR(fontSetBuilder->CreateFontSet(&fontSet));

	ComPtr<IDWriteFontCollection1> collection;
	HR(_writeFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &collection));

	return NewObject<SDWriteFontCollection>(this, std::move(collection));
}

IRHITextFormat* SDXGIFactory::CreateTextFormat(std::wstring_view fontFamilyName, IRHIFontCollection* fontCollection, ERHIFontWeight fontWeight, ERHIFontStyle fontStyle, ERHIFontStretch fontStretch, float fontSize, std::wstring_view localeName)
{
	IDWriteFontCollection* fontCollection_d = nullptr;

	if (auto fontCollection_s = Cast<SDWriteFontCollection>(fontCollection); fontCollection_s)
	{
		fontCollection_d = fontCollection_s->Get<IDWriteFontCollection>();
	}

	ComPtr<IDWriteTextFormat> format;
	HR(_writeFactory->CreateTextFormat(fontFamilyName.data(), fontCollection_d, (DWRITE_FONT_WEIGHT)fontWeight, (DWRITE_FONT_STYLE)fontStyle, (DWRITE_FONT_STRETCH)fontStretch, fontSize, localeName.data(), &format));

	return NewObject<SDWriteTextFormat>(this, std::move(format));
}

IRHITextLayout* SDXGIFactory::CreateTextLayout(IRHITextFormat* format, std::wstring_view text, const Vector2& layout)
{
	auto format_s = Cast<SDWriteTextFormat>(format);

	ComPtr<IDWriteTextLayout> textLayout;
	HR(_writeFactory->CreateTextLayout(text.data(), (UINT32)text.length(), format_s->Get<IDWriteTextFormat>(), layout.X, layout.Y, &textLayout));

	return NewObject<SDWriteTextLayout>(this, std::move(textLayout));
}