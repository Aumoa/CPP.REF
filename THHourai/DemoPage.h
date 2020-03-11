#pragma once

namespace Touhou
{
	class DemoPage : public BasePage
	{
		bool demoEnd = false;

		Diagnostics::StepTimer timer;
		
		RefPtr<Drawing::EaseFunction> myEase = new Drawing::EaseInOut( 1.0 );
		RefPtr<UI::TextBlock> helperText;

		RefPtr<UI::TextBlock> smartComponent;
		RefPtr<UI::TextBlock> gameEngine;

		RefPtr<IAsyncLoad> nextScene;

	public:
		DemoPage();

		void Update( Drawing::Rect<double> clientRect ) override;

		vs_property_get( bool, DemoEnd );
		bool DemoEnd_get();
		vs_property_set( String, HelpText );
		void HelpText_set( String value );

	private:
		void InitializeComponents();
	};
}