// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericSplash.h"

#if PLATFORM_WINDOWS

#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsCommon.h"
#include <optional>

namespace  /* Internal Storage */
{
	HWND hWnd;
	HANDLE hSplashThread;
	ULONG_PTR GdiplusToken;

	struct DrawingContext
	{
		Gdiplus::SolidBrush TextBrush;
		Gdiplus::SolidBrush TextShadowBrush;

		Gdiplus::FontFamily FontFamily;
		Gdiplus::Font Font;
		Gdiplus::Font RightFont;

		std::optional<Gdiplus::Bitmap> Buffer;
		std::optional<Gdiplus::Image> Splash;

		String AppText;
		String SplashText;

		DrawingContext()
			: TextBrush(Gdiplus::Color::White)
			, TextShadowBrush(Gdiplus::Color::Black)

			, FontFamily(L"Arial")
			, Font(&FontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel)
			, RightFont(&FontFamily, 9, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel)

			, AppText(String::Format(TEXT("AEEditor 0.1")))
			, SplashText(TEXT("Initializing..."))
		{
		}

		void GenerateBackBuffer(int32 nWidth, int32 nHeight)
		{
			Buffer.emplace(nWidth, nHeight);
		}
	};

	std::unique_ptr<DrawingContext> Drawing;

	void DrawSimplyShadowText(Gdiplus::Graphics& Context, String Text, Gdiplus::Font& Font, const Gdiplus::PointF& Position)
	{
		using namespace Gdiplus;

		// Draw background text passes
		constexpr int32 NumBGPasses = 8;
		for (int32 Pass = 0; Pass < NumBGPasses; ++Pass)
		{
			float BGXOffset, BGYOffset;
			switch (Pass)
			{
				default:
				case 0:	BGXOffset = -1.0f; BGYOffset = 0.0f; break;
				case 2:	BGXOffset = -1.0f; BGYOffset = -1.0f; break;
				case 3:	BGXOffset = 0.0f; BGYOffset = -1.0f; break;
				case 4:	BGXOffset = 1.0f; BGYOffset = -1.0f; break;
				case 5:	BGXOffset = 1.0f; BGYOffset = 0.0f; break;
				case 6:	BGXOffset = 1.0f; BGYOffset = 1.0f; break;
				case 7:	BGXOffset = 0.0f; BGYOffset = 1.0f; break;
				case 8:	BGXOffset = -1.0f; BGYOffset = 1.0f; break;
			}

			Context.DrawString(Text.c_str(), (INT)Text.length(), &Font, Position + PointF(BGXOffset, BGXOffset), &Drawing->TextShadowBrush);
		}

		// Draw text.
		Context.DrawString(Text.c_str(), (INT)Text.length(), &Font, Position, &Drawing->TextBrush);
	}

	void DoPaint(HWND InHWnd)
	{
		using namespace Gdiplus;

		PAINTSTRUCT PS;
		HDC hDC = BeginPaint(InHWnd, &PS);

		Graphics BufferCtx(&Drawing->Buffer.value());
		BufferCtx.Clear(Gdiplus::Color::Gray);

		if (Drawing->Splash.has_value())
		{
			// Draw background splash image.
			BufferCtx.DrawImage(&Drawing->Splash.value(), 0, 0);
		}
				
		UINT Width = Drawing->Buffer->GetWidth();
		UINT Height = Drawing->Buffer->GetHeight();
		PLATFORM_UNREFERENCED_PARAMETER(Width);

		PointF Position = PointF(10.0f, (float)Height - 50.0f - Drawing->Font.GetSize());
		DrawSimplyShadowText(BufferCtx, Drawing->AppText, Drawing->Font, Position);

		Position.Y = (float)Height - 30.0f - Drawing->RightFont.GetSize();
		DrawSimplyShadowText(BufferCtx, TEXT("Copyright 2020-2022 Aumoa.lib. All right reserved."), Drawing->RightFont, Position);

		Position.Y = (float)Height - 10.0f - Drawing->Font.GetSize();
		DrawSimplyShadowText(BufferCtx, Drawing->SplashText, Drawing->Font, Position);

		Graphics Context(hDC);
		Context.DrawImage(&Drawing->Buffer.value(), 0, 0);
	}

	LRESULT CALLBACK WndProc(HWND InHWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
			DoPaint(InHWnd);
			break;
		case WM_DESTROY:
			Drawing.reset();
			Gdiplus::GdiplusShutdown(GdiplusToken);

			// Sending quit message to splash thread.
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(InHWnd, uMsg, wParam, lParam);
	}

	void InitializeWindow()
	{
		using namespace Gdiplus;

		GdiplusStartupInput GSI;
		GdiplusStartup(&GdiplusToken, &GSI, NULL);

		WNDCLASSEX WndClass{};
		WndClass.cbSize = sizeof(WndClass);
		WndClass.lpfnWndProc = WndProc;
		WndClass.hInstance = GetModuleHandle(NULL);
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		WndClass.lpszClassName = TEXT("WindowsSplash").c_str();
		ATOM Ret = RegisterClassExW(&WndClass);
		checkf(Ret, TEXT("Failed to initialize window class for create splash window. Error: {}"), WindowsTools::GetErrorText());

		// Initial default screen size.
		int nWidth = 600;
		int nHeight = 200;

		// Initialize drawing context.
		Drawing = std::make_unique<DrawingContext>();

		// Load splash image.
		String SplashImageLoc = Path::Combine(Environment::GetEngineDirectory(), TEXT("Content"), TEXT("Splash"), TEXT("SplashImage.png"));
		if (File::Exists(SplashImageLoc))
		{
			Drawing->Splash.emplace(SplashImageLoc.c_str());
			nWidth = (int)Drawing->Splash->GetWidth();
			nHeight = (int)Drawing->Splash->GetHeight();
		}
		Drawing->GenerateBackBuffer(nWidth, nHeight);

		// Calculate window location.
		int nX = (GetSystemMetrics(SM_CXSCREEN) - nWidth) / 2;
		int nY = (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 2;

		UINT WndStyleEx = 0;
		// Force the splash screen to show up over taskbar.
		WndStyleEx |= WS_EX_APPWINDOW;
		// Force hide the splash screen in taskbar and alt-tab lists.
		WndStyleEx |= WS_EX_TOOLWINDOW;
		// Force allow fading the splash screen.
		WndStyleEx |= WS_EX_LAYERED;

		// Start the new thread for handing messages for splash window.
		auto CS = TaskCompletionSource<>::Create();
		std::thread([&CS, &WndClass, &WndStyleEx, &nX, &nY, &nWidth, &nHeight]()
		{
			hWnd = CreateWindowEx(WndStyleEx, WndClass.lpszClassName, NULL, WS_POPUP, nX, nY, nWidth, nHeight, NULL, NULL, WndClass.hInstance, NULL);
			checkf(hWnd, TEXT("Failed to create splash window. Error: {}"), WindowsTools::GetErrorText());

			// Set window to fully transparent to start out.
			SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);

			// In this point, all captured variables cannot be use.
			CS.SetResult();

			bool bFadingFinished = false;
			float FadingOpacity = 0;
			DateTime FadingPoint = DateTime::Now();
			constexpr double FadingDuration = 0.5;

			MSG M;
			while (hWnd)
			{
				// If is still fading, doesn't wait for message is received.
				if (bFadingFinished == false && PeekMessage(&M, hWnd, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&M);
					DispatchMessage(&M);

					if (M.message == WM_QUIT)
					{
						// Exit pumping thread.
						break;
					}
				}
				// Otherwise, waiting next message.
				else if (bFadingFinished)
				{
					if (GetMessage(&M, hWnd, 0, 0))
					{
						TranslateMessage(&M);
						DispatchMessage(&M);
					}
					else
					{
						break;
					}
				}
				else
				{
					if (bFadingFinished == false)
					{
						auto Current = DateTime::Now();
						auto DeltaSeconds = (Current - FadingPoint).GetTotalSeconds<double>();
						FadingPoint = Current;

						FadingOpacity += (float)(DeltaSeconds / FadingDuration);
						if (FadingOpacity >= 1.0f)
						{
							FadingOpacity = 1.0f;
							bFadingFinished = true;
						}

						// Convert alpha opacity to byte value.
						BYTE ByteOpacity = (BYTE)(255.0f * FadingOpacity);
						SetLayeredWindowAttributes(hWnd, 0, ByteOpacity, LWA_ALPHA);
					}
				}

				// Waiting for little time for yield thread-times.
				Sleep(30);
			}
		}).detach();

		CS.GetTask().Wait();
	}
}

void NGenericSplash::Show()
{
	if (hWnd == NULL)
	{
		InitializeWindow();
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

void NGenericSplash::Hide()
{
	if (hWnd != NULL)
	{
		// Send message to splash screen window to destroy itself.
		PostMessageW(hWnd, WM_DESTROY, 0, 0);
		hWnd = NULL;
	}

	if (hSplashThread)
	{
		WaitForSingleObject(hSplashThread, INFINITE);
		CloseHandle(hSplashThread);
		hSplashThread = NULL;
	}
}

void NGenericSplash::SetSplashText(String InText)
{
	check(hWnd);
	check(Drawing);

	Drawing->SplashText = InText;
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

#endif