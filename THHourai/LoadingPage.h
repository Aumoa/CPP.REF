#pragma once

namespace Touhou
{
	class LoadingPage : public BasePage
	{
		static RefPtr<Game::UI::TextFormat> textFormat;

		RefPtr<UI::Border> loadingBarBorder;
		RefPtr<Game::UI::Shapes::Rectangle> loadingBar;
		String text;
		RefPtr<Game::UI::TextBlock> textBlock;
		RefPtr<Game::UI::TextBlock> progressText;

		RefPtr<Diagnostics::StepTimer> fixedTimer;

		RefPtr<IAsyncLoad> asyncLoad;

	public:
		LoadingPage();

		void Update( Drawing::Rect<double> clientRect ) override;

		vs_property( String, Text );
		vs_property( RefPtr<IAsyncLoad>, AsyncLoad );

		String Text_get();
		void Text_set( String value );
		RefPtr<IAsyncLoad> AsyncLoad_get();
		void AsyncLoad_set( RefPtr<IAsyncLoad> value );

	private:
		void InitializeComponents();
	};
}