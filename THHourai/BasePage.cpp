using namespace Touhou;

using namespace SC::Diagnostics;
using namespace SC::Drawing;
using namespace SC::Game::UI;

object globalDebug;

RefPtr<TextBlock> BasePage::debugText;
RefPtr<TextBlock> BasePage::fpsText;

RefPtr<Element> BasePage::Content_get()
{
	return content;
}

void BasePage::Content_set( RefPtr<Element> value )
{
	if ( content )
	{
		panel->Remove( value );
	}

	panel->Add( value );
}

BasePage::BasePage()
{
	InitializeComponents();
}

void BasePage::Update( Rect<double> clientRect )
{
	fpsTimer->Tick();

	debugText->Content = globalDebug;

	fpsChecker += fpsTimer->ElapsedSeconds;
	if ( fpsChecker >= 1.0 )
	{
		fpsChecker -= 1.0;
		auto text = String::Format( "FPS: {0}\nTime: {1}ms\n{2}", fpsTimer->TicksPerSecond, fpsTimer->ElapsedSeconds * 1000, App::Instance()->AppConfig.AdapterName );
		fpsText->Content = text;
		//App::Instance()->AppName = text;
	}

	Page::Update( clientRect );
}

void BasePage::SetDebugText( object stringConvertible )
{
	if ( debugText )
	{
		debugText->Content = stringConvertible;
	}
}

void BasePage::InitializeComponents()
{
	fpsTimer = new StepTimer();

	declare_element( RelativePanel, relativePanel );
	{
		if ( !this->fpsText )
		{
			declare_element( TextBlock, fpsText );
			fpsText->Content = "FPS: 0\nTime: 0ms";
			fpsText->Color = Color::White;
			fpsText->Margin = 10;
			fpsText->Anchor = Anchor::RightTop;
			fpsText->AddDependencyProperty( "Panel.ZOrder=100" );
			this->fpsText = fpsText;
		}
		relativePanel->Add( fpsText );

		if ( !this->debugText )
		{
			declare_element( TextBlock, debugText );
			debugText->AddDependencyProperty( DependencyProperty( "RelativePanel.Below=fpsText" ) );
			debugText->Margin = 10;
			debugText->Color = Color::White;
			debugText->AddDependencyProperty( "Panel.ZOrder=100" );
			fpsText->Anchor = Anchor::RightTop;
			this->debugText = debugText;
		}
		relativePanel->Add( debugText );
	}
	this->panel = relativePanel;

	Page::Content = panel;
}