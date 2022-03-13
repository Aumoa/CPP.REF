// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WindowsApplication.h"
#include "GameModule.h"
#include "GameEngine.h"
#include "Input/WindowsPlatformKeyboard.h"
#include "Input/WindowsPlatformMouse.h"
#include "Input/WindowsIMEController.h"
#include "Multimedia/WindowsImage.h"
#include "Misc/CommandLine.h"
#include "Misc/PlatformModule.h"
#include "Diagnostics/LogModule.h"

// Vulkan
//#include "VkFactory.h"
// DirectX
#include "DirectXFactory.h"

GENERATE_BODY(SWindowsApplication);

#define WM_UPDATETICKMODE		WM_APP + 1
#define WM_PRESENT				WM_UPDATETICKMODE + 1

SWindowsApplication::SWindowsApplication(HINSTANCE hInstance) : Super()
	, hInstance(hInstance)
{
	checkf(gApp == nullptr, L"Singleton instance duplicated.");
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

	PlatformKeyboard = gcnew SWindowsPlatformKeyboard();
	PlatformMouse = gcnew SWindowsPlatformMouse();
	PlatformIME = gcnew SWindowsIMEController();
}

int32 SWindowsApplication::GuardedMain(std::span<const std::wstring> Argv)
{
	int32 ErrorCode;

	auto Logger = std::make_unique<LogModule>(String::AsUnicode(SE_APPLICATION));
	Logger->RunTask();

	std::vector<std::unique_ptr<PlatformModule>> PlatformModules;

	{
		GC.Init();

		auto CommandArgs = CommandLine(Argv);
		std::wstring ModuleName;

		if (!CommandArgs.TryGetValue(L"GameDll", ModuleName))
		{
			ModuleName = String::AsUnicode(SE_APPLICATION_TARGET);
		}

		std::wstring EngineName;
		if (!CommandArgs.TryGetValue(L"EngineDll", EngineName))
		{
#if !SHIPPING
			constexpr const wchar_t* GameEngineModuleName = L"Editor.dll";
#else
			constexpr const wchar_t* GameEngineModuleName = L"Game.dll";
#endif
			EngineName = GameEngineModuleName;
		}

		std::unique_ptr EngineModule = std::make_unique<PlatformModule>(EngineName);
		auto Loader = EngineModule->GetFunctionPointer<SGameModule*()>("LoadGameModule");
		if (!Loader)
		{
			SE_LOG(LogWindows, Fatal, L"GameEngine does not initialized. {} is corrupted.", EngineName);
			return -1;
		}

		SharedPtr<SGameModule> GameModule = Loader();
		if (GameModule == nullptr)
		{
			SE_LOG(LogWindows, Fatal, L"LoadGameModule function does not defined. Please DEFINE_GAME_MODULE to any code file in module project to provide loader.");
			return -1;
		}

		SharedPtr<SGameEngine> GameEngine = GameModule->CreateGameEngine();
		if (GameEngine == nullptr)
		{
			SE_LOG(LogWindows, Fatal, L"Could not create GameEngine. CreateGameEngine function on GameModule return nullptr.");
			return -1;
		}

		SharedPtr WinApp = gcnew SWindowsApplication(GetModuleHandleW(nullptr));
		ErrorCode = GameEngine->GuardedMain(WinApp.Get(), ModuleName);
		if (ErrorCode != 0)
		{
			SE_LOG(LogWindows, Error, L"Application has one more error({}).", ErrorCode);
		}

		// Save platform modules for finish to dispose.
		PlatformModules = std::move(WinApp->PlatformModules);
		PlatformModules.emplace_back(std::move(EngineModule));
	}

	GC.Collect(true);
	GC.Shutdown(true);

	Logger->Shutdown();
	return ErrorCode;
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
				Idle.Broadcast(ActualTickMode);
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
			Idle.Broadcast(ActualTickMode);
		}
	}
}

void SWindowsApplication::ConsumeModule(std::unique_ptr<PlatformModule> ModulePtr)
{
	PlatformModules.emplace_back(std::move(ModulePtr));
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

void* SWindowsApplication::GetWindowHandle()
{
	return hWnd;
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
	RealtimeDemanders.emplace_back(InObject);
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
		if (!Holder.IsValid())
		{
			CompactList.emplace_back(i);
			continue;
		}

		if (Holder.Get() == InObject)
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
		Factory = gcnew SDirectXFactory(true);
	}
	return Factory;
}

IPlatformKeyboard& SWindowsApplication::GetPlatformKeyboard()
{
	return *PlatformKeyboard;
}

IPlatformMouse& SWindowsApplication::GetPlatformMouse()
{
	return *PlatformMouse;
}

IPlatformImageLoader& SWindowsApplication::GetPlatformImageLoader()
{
	return *this;
}

IPlatformIME& SWindowsApplication::GetPlatformIME()
{
	return *PlatformIME;
}

IPlatformImage* SWindowsApplication::CreateImageFromFile(const std::filesystem::path& InAssetPath, int32 FrameIndex, ERHIPixelFormat PixelFormat)
{
	ComPtr<IWICBitmapDecoder> Decoder;
	HR(ImagingFactory->CreateDecoderFromFilename(InAssetPath.wstring().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &Decoder));

	ComPtr<IWICFormatConverter> Converter = DecodeImage(Decoder.Get(), FrameIndex, PixelFormat);
	return gcnew SWindowsImage(Converter.Get(), PixelFormat);
}

IPlatformImage* SWindowsApplication::CreateImageFromBinary(std::span<const uint8> AssetsBin, int32 FrameIndex, ERHIPixelFormat PixelFormat)
{
	ComPtr<IWICStream> BinaryStream;
	HR(ImagingFactory->CreateStream(&BinaryStream));
	HR(BinaryStream->InitializeFromMemory((BYTE*)AssetsBin.data(), (DWORD)AssetsBin.size()));

	ComPtr<IWICBitmapDecoder> Decoder;
	HR(ImagingFactory->CreateDecoderFromStream(BinaryStream.Get(), NULL, WICDecodeMetadataCacheOnDemand, &Decoder));

	ComPtr<IWICFormatConverter> Converter = DecodeImage(Decoder.Get(), FrameIndex, PixelFormat);
	return gcnew SWindowsImage(Converter.Get(), PixelFormat);
}

void SWindowsApplication::ShrinkRealtimeDemanders()
{
	std::vector<size_t> CompactList;
	for (size_t i = 0; i < RealtimeDemanders.size(); ++i)
	{
		auto& Holder = RealtimeDemanders[i];
		if (!Holder.IsValid())
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

ComPtr<IWICFormatConverter> SWindowsApplication::DecodeImage(IWICBitmapDecoder* Decoder, int32 FrameIndex, ERHIPixelFormat PixelFormat)
{
	ComPtr<IWICBitmapFrameDecode> Frame;
	HR(Decoder->GetFrame(FrameIndex, &Frame));

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

	return Converter;
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
		gApp->Sized.Broadcast(Vector2N(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_IME_STARTCOMPOSITION:
		SWindowsIMEController::ProcessMessage(uMsg, wParam, lParam);
		return TRUE;
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_SETCONTEXT:
	case WM_IME_NOTIFY:
		SWindowsIMEController::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_CLOSE:
		Get().PreDestroyApp.Broadcast();
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}