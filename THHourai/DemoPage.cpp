using namespace Touhou;

using namespace SC::Game::UI;
using namespace SC::Drawing;
using namespace SC::Threading;

DemoPage::DemoPage()
{
	myEase->Start();
	InitializeComponents();
}

void DemoPage::Update( Drawing::Rect<double> clientRect )
{
	timer.Tick();

#if defined( _DEBUG )
	demoEnd = true;
#else
	if ( timer.TotalSeconds >= 5.0 )
	{
		demoEnd = true;
	}
	else if ( timer.TotalSeconds >= 3.0 )
	{
		double t = ( timer.TotalSeconds - 3.0 ) * 0.5;
		smartComponent->Opacity = 1.0 - t;
		gameEngine->Opacity = 1.0 - t;
	}
	else if ( timer.TotalSeconds >= 2.0 )
	{
		double t = 1.0;
		myEase->FixedUpdate( t );

		double height = smartComponent->ActualContentRect.Height;

		smartComponent->Margin = Thickness( 0, -myEase->CurrentValue * height, 0, 0 );
		gameEngine->Margin = Thickness( 0, myEase->CurrentValue * height, 0, 0 );
		gameEngine->Opacity = ( myEase->CurrentValue - 0.5 ) * 2.0;
	}
	else if ( timer.TotalSeconds >= 1.0 )
	{
		double t = timer.TotalSeconds - 1.0;
		myEase->FixedUpdate( t );

		double height = smartComponent->ActualContentRect.Height;

		smartComponent->Margin = Thickness( 0, -myEase->CurrentValue * height, 0, 0 );
		gameEngine->Margin = Thickness( 0, myEase->CurrentValue * height, 0, 0 );
		gameEngine->Opacity = ( myEase->CurrentValue - 0.5 ) * 2.0;
	}
#endif

	BasePage::Update( clientRect );
}

bool DemoPage::DemoEnd_get()
{
	return demoEnd;
}

void DemoPage::HelpText_set( String value )
{
	helperText->Content = value;
}

void DemoPage::InitializeComponents()
{
	declare_element( RelativePanel, relativePanel );
	{
		declare_element( TextBlock, smartComponent );
		smartComponent->Content = "Smart Component";
		smartComponent->Anchor = Anchor::Center;
		smartComponent->Color = Color::White;
		smartComponent->Format = new TextFormat( "", 50 );
		relativePanel->Add( smartComponent );
		this->smartComponent = smartComponent;

		declare_element( TextBlock, gameEngine );
		gameEngine->Content = "<b>GAME ENGINE</b>";
		gameEngine->AutoSizing = false;
		gameEngine->AddDependencyProperty( "RelativePanel.AlignTopWith=smartComponent" );
		gameEngine->Height = 100;
		gameEngine->Format = new TextFormat( "", 60 );
		gameEngine->Color = Color::White;
		gameEngine->Alignment = TextAlignment::Center;
		gameEngine->Opacity = 0;
		relativePanel->Add( gameEngine );
		this->gameEngine = gameEngine;

		declare_element( TextBlock, helperText );
		helperText->Content = "";
		helperText->Anchor = Anchor::LeftBottom;
		helperText->Color = Color::White;
		relativePanel->Add( helperText );
		this->helperText = helperText;
	}

	Content = relativePanel;
}