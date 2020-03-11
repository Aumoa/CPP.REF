#pragma once

namespace Touhou
{
	class BasePage : public Game::UI::Page
	{
		RefPtr<Game::UI::Element> content;
		RefPtr<Game::UI::RelativePanel> panel;

		static RefPtr<Game::UI::TextBlock> debugText;
		static RefPtr<Game::UI::TextBlock> fpsText;

		RefPtr<Diagnostics::StepTimer> fpsTimer;
		double fpsChecker;

	protected:
		vs_property( RefPtr<Game::UI::Element>, Content );
		RefPtr<Game::UI::Element> Content_get();
		void Content_set( RefPtr<Game::UI::Element> value );

	public:
		BasePage();

		void Update( Drawing::Rect<double> clientRect ) override;

		void SetDebugText( object stringConvertible );

	private:
		void InitializeComponents();
	};
}