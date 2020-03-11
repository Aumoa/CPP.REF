#pragma once

namespace SC::Game::Details
{
	struct tag_GlobalVar
	{
		Application* pApp;
		HWND hWnd;
		String systemMessageFontName;

		RefPtr<IntegratedFactory> factory;
		RefPtr<CDevice> device;
		RefPtr<CSwapChain> swapChain;
		RefPtr<GameLogic> gameLogic;

		RefPtr<UI::TextFormat> defaultTextFormat;
		std::set<GlyphBuffer*> glyphBuffers;

		int frameIndex = 0;
		int fixedFrameIndex = 0;
	};

	extern tag_GlobalVar GlobalVar;
	extern bool AppShutdown;
	extern GarbageCollector GC;
}