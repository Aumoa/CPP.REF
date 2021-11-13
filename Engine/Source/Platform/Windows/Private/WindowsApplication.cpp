// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsApplication.h"
#include "Input/WindowsPlatformKeyboard.h"
#include "Input/WindowsPlatformMouse.h"
#include "D3D12RHI/DXGIFactory.h"
#include "Multimedia/WindowsImage.h"

#define WM_UPDATETICKMODE		WM_APP + 1
#define WM_PRESENT				WM_UPDATETICKMODE + 1

SWindowsApplication::SWindowsApplication(HINSTANCE hInstance) : Super()
	, hInstance(hInstance)
{
	check(gApp == nullptr);
	gApp = this;

	WNDCLASSEXW Wcex = {};
	Wcex.cbSize = sizeof(Wcex);
	Wcex.lpfnWndProc = WndProc;
	Wcex.lpszClassName = L"WindowsApplication";
	Wcex.hInstance = hInstance;
	if (RegisterClassExW(&Wcex) == 0)
	{
		ReportWindowsError(L"Could not register window class.");
	}

	hWnd = CreateWindowExW(0, Wcex.lpszClassName, L"WindowsApplication", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (hWnd == nullptr)
	{
		ReportWindowsError(L"Could not create core window.");
	}

	HR(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	HR(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ImagingFactory)));
}

void SWindowsApplication::Start()
{
	ShowWindow(hWnd, SW_SHOW);

	MSG Msg = {};

	while (true)
	{
		if (ActualTickMode == ETickMode::Realtime)
		{
			if (PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (Msg.message == WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&Msg);
					DispatchMessageW(&Msg);
				}
			}
			else
			{
				Idle.Invoke(ActualTickMode);
			}
		}
		else
		{
			if (!GetMessageW(&Msg, nullptr, 0, 0))
			{
				break;
			}
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
			Idle.Invoke(ActualTickMode);
		}
	}
}

Vector2N SWindowsApplication::GetViewportSize()
{
	RECT Rc;
	GetClientRect(hWnd, &Rc);
	return Vector2N(Rc.right - Rc.left, Rc.bottom - Rc.top);
}

float SWindowsApplication::GetDpi()
{
	return (float)GetDpiForWindow(hWnd);
}

void SWindowsApplication::SetTickMode(ETickMode InTickMode)
{
	if (TickMode != InTickMode)
	{
		TickMode = InTickMode;
		UpdateRealtimeDemanders();
	}
}

auto SWindowsApplication::GetTickMode() -> ETickMode
{
	return TickMode;
}

void SWindowsApplication::AddRealtimeDemander(SObject* InObject)
{
	RealtimeDemanders.emplace_back(InObject->WeakFromThis());
	ShrinkRealtimeDemanders();
	UpdateRealtimeDemanders();
}

void SWindowsApplication::RemoveRealtimeDemander(SObject* InObject)
{
	std::vector<size_t> CompactList;
	bool bFound = false;

	for (size_t i = 0; i < RealtimeDemanders.size(); ++i)
	{
		auto& Holder = RealtimeDemanders[i];
		if (Holder.expired())
		{
			CompactList.emplace_back(i);
			continue;
		}

		auto Ptr = Holder.lock();
		if (Ptr.get() == InObject)
		{
			CompactList.emplace_back(i);
			bFound = true;
			break;
		}
	}

	size_t NumRemoves = 0;
	for (auto& Id : CompactList)
	{
		size_t Index = Id - NumRemoves++;
		RealtimeDemanders.erase(RealtimeDemanders.begin() + Index);
	}

	UpdateRealtimeDemanders();
}

void SWindowsApplication::SetTitle(std::wstring_view InTitle)
{
	SetWindowTextW(hWnd, InTitle.data());
}

std::wstring SWindowsApplication::GetTitle()
{
	int32 Length = GetWindowTextLengthW(hWnd);
	std::wstring Buf;
	Buf.resize(Length);

	GetWindowTextW(hWnd, Buf.data(), Length);
	return Buf;
}

IRHIFactory* SWindowsApplication::GetFactory()
{
	if (Factory == nullptr)
	{
		Factory = NewObject<SDXGIFactory>(this);
	}
	return Factory;
}

IPlatformKeyboard& SWindowsApplication::GetPlatformKeyboard()
{
	return SWindowsPlatformKeyboard::Get();
}

IPlatformMouse& SWindowsApplication::GetPlatformMouse()
{
	return SWindowsPlatformMouse::Get();
}

IPlatformImageLoader& SWindowsApplication::GetPlatformImageLoader()
{
	return *this;
}

IPlatformImage* SWindowsApplication::CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat)
{
	ComPtr<IWICBitmapDecoder> Decoder;
	HR(ImagingFactory->CreateDecoderFromFilename(InAssetPath.wstring().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &Decoder));

	ComPtr<IWICBitmapFrameDecode> Frame;
	HR(Decoder->GetFrame(0, &Frame));

	WICPixelFormatGUID FormatGUID;

	switch (PixelFormat)
	{
	case ERHIPixelFormat::B8G8R8A8_UNORM:
		FormatGUID = GUID_WICPixelFormat32bppPBGRA;
		break;
	case ERHIPixelFormat::R8G8B8A8_UNORM:
		FormatGUID = GUID_WICPixelFormat32bppPRGBA;
		break;
	default:
		SE_LOG(LogWindows, Fatal, L"Not supported pixel format.");
	}

	ComPtr<IWICFormatConverter> Converter;
	HR(ImagingFactory->CreateFormatConverter(&Converter));
	HR(Converter->Initialize(Frame.Get(), FormatGUID, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom));

	return NewObject<SWindowsImage>(Converter.Get(), PixelFormat);
}

HWND SWindowsApplication::GetWindowHandle()
{
	return hWnd;
}

void SWindowsApplication::ShrinkRealtimeDemanders()
{
	std::vector<size_t> CompactList;
	for (size_t i = 0; i < RealtimeDemanders.size(); ++i)
	{
		auto& Holder = RealtimeDemanders[i];
		if (Holder.expired())
		{
			CompactList.emplace_back(i);
			continue;
		}
	}

	size_t NumRemoves = 0;
	for (auto& Id : CompactList)
	{
		size_t Index = Id - NumRemoves++;
		RealtimeDemanders.erase(RealtimeDemanders.begin() + Index);
	}
}

void SWindowsApplication::UpdateRealtimeDemanders()
{
	if (RealtimeDemanders.size())
	{
		ActualTickMode = ETickMode::Realtime;
	}
	else
	{
		ActualTickMode = TickMode;
	}

	PostMessageW(hWnd, WM_UPDATETICKMODE, 0, 0);
}

LRESULT CALLBACK SWindowsApplication::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		SWindowsPlatformMouse::ProcessMessage(uMsg, wParam, lParam);
		SWindowsPlatformKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		SWindowsPlatformMouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		SWindowsPlatformKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_SIZE:
		gApp->Sized.Invoke(Vector2N(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}