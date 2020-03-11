using namespace Touhou;

using namespace SC::Game::UI;
using namespace SC::Drawing;

inline namespace
{
	class GridViewItem : public Button
	{
		EventHandler<int> pickCallback;
		int pickId;

	public:
		GridViewItem( int id, String name, RefPtr<Texture2D> image, String text, EventHandler<int> pickCallback ) : Button( name )
			, pickCallback( pickCallback )
			, pickId( id )
		{
			declare_element( FixedPanel, fixedPanel );
			{
				declare_element( Image, frame );
				frame->Source = image;
				frame->Width = 100;
				frame->Height = 100;
				frame->Margin = 5;
				fixedPanel->Add( frame );

				declare_element( TextBlock, subtitle );
				subtitle->Content = text;
				subtitle->Width = 100;
				subtitle->Height = 40;
				subtitle->AutoSizing = false;
				subtitle->Alignment = TextAlignment::Center;
				subtitle->VerticalAlignment = TextVerticalAlignment::Center;
				subtitle->Color = Color::White;
				subtitle->Margin = Thickness( 5, 105, 5, 5 );
				fixedPanel->Add( subtitle );
			}

			Width = 110;
			Height = 150;
			Margin = Thickness( 5, 5 );

			Content = fixedPanel;
			Clicked += EventHandler<>( this, &GridViewItem::OnButtonDown );
		}

	protected:
		void OnButtonDown( object sender )
		{
			pickCallback( this, pickId );
		}
	};
}

CharacterPickPage::CharacterPickPage() : BasePage()
{
	InitializeComponents();
}

void CharacterPickPage::Update( Rect<double> clientRect )
{
	if ( !shutdown )
	{
		if ( !timer )
		{
			timer = new Diagnostics::StepTimer();
		}
		timer->Tick();

		if ( !finish )
		{
			if ( timer->TotalSeconds >= 1 )
			{
				fadeOutBrush->Opacity = 0.0;
				finish = true;
			}
			else
			{
				double t = timer->TotalSeconds;
				fadeOutBrush->Opacity = 1.0 - t;
			}
		}
	}
	else
	{
		if ( !timer )
		{
			timer = new Diagnostics::StepTimer();
			finish = false;
		}
		timer->Tick();

		if ( !finish )
		{
			if ( timer->TotalSeconds >= 2 )
			{
				fadeOutBrush->Opacity = 1.0;
				Finish( nullptr );
				finish = true;
			}
			else
			{
				double t = timer->TotalSeconds;
				fadeOutBrush->Opacity = t * 0.5;
			}
		}
	}

	return BasePage::Update( clientRect );
}

void CharacterPickPage::InitializeComponents()
{
	var textFormat = new TextFormat( "", 24 );

	RefPtr kaguya_portrait = new Texture2D( "kaguya_portrait", "Assets/Image/Kaguya_Portrait.jpg" );
	RefPtr mokou_portrait = new Texture2D( "mokou_portrait", "Assets/Image/Mokou_Portrait.jpg" );

	fadeOutBrush = new SolidColorBrush( Color::Black );

	auto handler = EventHandler<int>( this, &CharacterPickPage::OnPick );

	declare_element( RelativePanel, relativePanel );
	{
		declare_element( Shapes::Rectangle, fadeOut );
		fadeOut->Fill = fadeOutBrush;
		fadeOut->Width = 0;
		fadeOut->Height = 0;
		fadeOut->AddDependencyProperty( "Panel.ZOrder=-1" );
		relativePanel->Add( fadeOut );
		this->fadeOut = fadeOut;

		declare_element( TextBlock, title );
		title->Content = "캐릭터를 선택하세요.";
		title->Format = textFormat;
		title->Margin = Thickness( 25, 50, 0, 0 );
		title->Color = Color::White;
		relativePanel->Add( title );

		declare_element( GridView, gridView );
		{
			gridView->Add( new GridViewItem( 0, "kaguyaItem", kaguya_portrait, "호라이산\n카구야", handler ) );
			gridView->Add( new GridViewItem( 1, "mokouItem", mokou_portrait, "후지와라노\n모코우", handler ) );
		}
		gridView->AddDependencyProperty( "RelativePanel.AlignLeftWith=title" );
		gridView->AddDependencyProperty( "RelativePanel.Below=title" );
		gridView->GridWidth = 120;
		gridView->GridHeight = 160;
		gridView->Margin = Thickness( 0, 25 );
		relativePanel->Add( gridView );

		declare_element( TextBlock, pickText );
		pickText->Content = "선택";
		pickText->AutoSizing = false;
		pickText->Width = 0;
		pickText->Height = 0;
		pickText->Color = Color::White;
		pickText->Format = textFormat;
		pickText->Alignment = TextAlignment::Center;
		pickText->VerticalAlignment = TextVerticalAlignment::Center;
		pickText->Margin = 1;

		declare_element( Button, pick );
		pick->Content = pickText;
		pick->Width = 120;
		pick->Height = 60;
		pick->Anchor = Anchor::LeftBottom;
		pick->Margin = 25;
		this->pickButton = pick;
	}
	this->relativePanel = relativePanel;

	Content = relativePanel;
}

void CharacterPickPage::OnPick( object button, int pickId )
{
	if ( !shutdown )
	{
		Picked( button, pickId );

		if ( !buttonIncluded )
		{
			relativePanel->Add( pickButton );
			buttonIncluded = true;
			pickButton->Clicked += EventHandler<>( this, &CharacterPickPage::OnPick );
		}
	}
}

void CharacterPickPage::OnPick( object button )
{
	if ( !shutdown )
	{
		Choice( button );

		fadeOut->RemoveDependencyProperty( Panel::ClassName, Panel::ZOrder );
		fadeOut->AddDependencyProperty( "Panel.ZOrder=1" );

		shutdown = true;
		timer = nullptr;
	}
}