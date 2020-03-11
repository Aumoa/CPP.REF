#pragma once

namespace Touhou
{
	class CharacterPickPage : public BasePage
	{
		RefPtr<Diagnostics::StepTimer> timer;
		bool finish = false;
		RefPtr<UI::Shapes::Rectangle> fadeOut;
		RefPtr<UI::SolidColorBrush> fadeOutBrush;

		RefPtr<UI::RelativePanel> relativePanel;
		RefPtr<UI::Button> pickButton;
		bool buttonIncluded = false;

		bool shutdown = false;

	public:
		CharacterPickPage();

		SC::Event<int> Picked;
		SC::Event<> Choice;
		SC::Event<> Finish;

	private:
		void InitializeComponents();

		void Update( Drawing::Rect<double> clientRect ) override;

		void OnPick( object button, int pickId );
		void OnPick( object button );
	};
}