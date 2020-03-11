using namespace Touhou;

using namespace SC::Game::UI;
using namespace SC::Drawing;

RefPtr<TextFormat> OnlyScriptPage::textFormatContinue;
RefPtr<TextFormat> OnlyScriptPage::textFormatScript;

OnlyScriptPage::OnlyScriptPage( int numScripts ) : BasePage()
	, numScripts( numScripts )
{
	textSequences.resize( numScripts );
	textScripts.resize( numScripts );

	InitializeComponents();

	dropdownTick.IsFixedTimeStep = true;
	dropdownTick.TargetElapsedSeconds = 0.3;
}

void OnlyScriptPage::Update( Drawing::Rect<double> clientRect )
{
	dropdownTick.Tick( [&]()
		{
			dropdownState = !dropdownState;
			dropdownText->Opacity = ( double )dropdownState;
		}
	);

	dropdownText->RemoveDependencyProperty( RelativePanel::ClassName );
	dropdownText->AddDependencyProperty( String::Format( "RelativePanel.RightOf={0}", currentText->Name ) );
	dropdownText->AddDependencyProperty( String::Format( "RelativePanel.AlignTopWith={0}", currentText->Name ) );
	currentText->Name;

	BasePage::Update( clientRect );
}

void OnlyScriptPage::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	switch ( args->Type )
	{
	case DispatcherEventType::MouseClick:
		if ( auto evt = args->Argument.As<MouseClickEventArgs>(); evt.IsDown == false )
		{
			// 마우스 버튼 놓음 이벤트에서는 입력을 스킵한다.
			break;
		}
		MoveNext();
		break;
	case DispatcherEventType::KeyboardEvent:
		if ( auto evt = args->Argument.As<KeyboardEventArgs>(); evt.IsDown == false )
		{
			// 키보드 놓음 이벤트에서는 입력을 스킵한다.
			break;
		}

		MoveNext();
		break;
	}
}

String OnlyScriptPage::Script_get( int param0 )
{
	return textScripts[param0];
}

void OnlyScriptPage::Script_set( int param0, String value )
{
	textScripts[param0] = value;
	textSequences[param0]->Content = value;
}

void OnlyScriptPage::HelpText_set( String value )
{
	helpText->Content = value;
}

void OnlyScriptPage::InitializeComponents()
{
	if ( !textFormatContinue )
	{
		textFormatContinue = new TextFormat( "", 22.0 );
	}

	if ( !textFormatScript )
	{
		textFormatScript = new TextFormat( "", 20.0 );
	}

	declare_element( RelativePanel, relativePanel );
	{
		for ( int i = 0; i < numScripts; ++i )
		{
			RefPtr textBlock = new TextBlock( String::Format( "textSequences[{0}]", i ) );
			textBlock->Content = "";
			textBlock->Margin = Thickness( 30, i == 0 ? 100 : 16, 30, 0 );
			textBlock->Color = Color::White;
			textBlock->Alignment = TextAlignment::Left;
			textBlock->Format = textFormatScript;
			if ( i != 0 )
			{
				textBlock->AddDependencyProperty( String::Format( "RelativePanel.Below={0}", this->textSequences[i - 1]->Name ) );
				textBlock->Opacity = 0;
			}
			relativePanel->Add( textBlock );
			this->textSequences[i] = textBlock;
		}

		declare_element( TextBlock, dropdownText );
		dropdownText->Content = " ▼";
		dropdownText->Color = Color::White;
		relativePanel->Add( dropdownText );
		this->dropdownText = dropdownText;

		declare_element( TextBlock, helpText );
		helpText->Format = textFormatContinue;
		helpText->Content = "아무 키나 눌러 계속 진행";
		helpText->Margin = 25.0;
		helpText->Anchor = Anchor::RightBottom;
		helpText->Color = Color::White;
		relativePanel->Add( helpText );
		this->helpText = helpText;

		currentText = this->textSequences[0];
	}

	Content = relativePanel;
}

void OnlyScriptPage::MoveNext()
{
	if ( currentSeqIndex == numScripts - 1 )
	{
		ScriptEnded( this );
	}
	else
	{
		currentSeqIndex += 1;
		currentText = textSequences[currentSeqIndex];
		currentText->Opacity = 1;
	}
}