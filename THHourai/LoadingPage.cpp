using namespace Touhou;

using namespace SC::Diagnostics;
using namespace SC::Game;
using namespace SC::Game::UI;
using namespace SC::Drawing;

RefPtr<Game::UI::TextFormat> LoadingPage::textFormat;

LoadingPage::LoadingPage()
{
	InitializeComponents();

	Text = "NOW LOADING";

	fixedTimer = new StepTimer();
}

void LoadingPage::Update( Rect<double> clientRect )
{
	fixedTimer->Tick( [&]()
		{
			if ( !fixedTimer->IsFixedTimeStep )
			{
				fixedTimer->IsFixedTimeStep = true;
				fixedTimer->TargetElapsedTicksPerSecond = 30;
			}

			double score = 0;
			if ( asyncLoad ) score = asyncLoad->Progress;
			score = std::clamp( score, 0.0001, 1.0 );

			static int dotCount = 0;

			dotCount += 1;
			if ( dotCount > 39 )
			{
				dotCount = 0;
			}

			const char* dots[4] = { "", ".", "..", "..." };
			textBlock->Content = String::Format( "{0}{1}", text, dots[dotCount / 10] );

			auto actual = loadingBarBorder->ActualContentRect;

			auto padding = loadingBarBorder->Padding;
			actual.Left += padding.Left;
			actual.Top += padding.Top;
			actual.Right -= padding.Right;
			actual.Bottom -= padding.Bottom;

			auto length = actual.Width;
			actual.Right = actual.Left + length * score;

			loadingBar->Width = actual.Width;
			loadingBar->Height = actual.Height;

			std::wstringstream wss;
			wss.precision( 2 );
			wss << std::fixed << score * 100 << L"%";
			progressText->Content = wss.str();
		}
	);

	return BasePage::Update( clientRect );
}

String LoadingPage::Text_get()
{
	return text;
}

void LoadingPage::Text_set( String value )
{
	text = value;
}

RefPtr<IAsyncLoad> LoadingPage::AsyncLoad_get()
{
	return asyncLoad;
}

void LoadingPage::AsyncLoad_set( RefPtr<IAsyncLoad> value )
{
	asyncLoad = value;
}

void LoadingPage::InitializeComponents()
{
	var revealBrush = new RevealBrush( Color::Red );
	revealBrush->MinOpacity = 0.3;
	revealBrush->RevealDistance = 300.0;

	declare_element( RelativePanel, panel );
	{
		if ( !textFormat )
		{
			textFormat = new TextFormat( "", 25.0 );
		}

		declare_element( Border, loadingBarBorder );
		loadingBarBorder->Height = 30;
		loadingBarBorder->Anchor = Anchor::Bottom;
		loadingBarBorder->Margin = 15;
		loadingBarBorder->Padding = Thickness( 5, 5, 5, 5 );
		loadingBarBorder->Fill = revealBrush;
		{
			declare_element( FixedPanel, fixedPanel );
			{
				declare_element( Shapes::Rectangle, loadingBackground );
				loadingBackground->Width = 0;
				loadingBackground->Height = 0;
				loadingBackground->Fill = new SolidColorBrush( Color::Black );
				fixedPanel->Add( loadingBackground );

				declare_element( Shapes::Rectangle, loadingBar );
				loadingBar->Width = 0;
				loadingBar->Height = 0;
				loadingBar->Fill = new SolidColorBrush( Color::White );
				this->loadingBar = loadingBar;
				fixedPanel->Add( loadingBar );

				declare_element( TextBlock, progressText );
				progressText->Content = "0.01%";
				progressText->Anchor = Anchor::Right;
				progressText->Color = Color::LightGray;
				progressText->Format = textFormat;
				this->progressText = progressText;
				fixedPanel->Add( progressText );
			}
			loadingBarBorder->Content = fixedPanel;
		}
		this->loadingBarBorder = loadingBarBorder;
		panel->Add( loadingBarBorder );

		declare_element( TextBlock, nowLoadingText );
		nowLoadingText->Format = textFormat;
		nowLoadingText->Content = "NOW LOADING";
		nowLoadingText->Anchor = Anchor::Bottom;
		nowLoadingText->Color = Color::White;
		nowLoadingText->AddDependencyProperty( "RelativePanel.Above=loadingBarBorder" );
		this->textBlock = nowLoadingText;
		panel->Add( nowLoadingText );
	}

	Content = panel;
}