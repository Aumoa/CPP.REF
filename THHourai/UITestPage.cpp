using namespace Touhou;

using namespace SC::Game::UI;
using namespace SC::Drawing;

UITestPage::UITestPage() : BasePage()
{
	InitializeComponents();
}

void UITestPage::InitializeComponents()
{
	declare_element( RelativePanel, relativePanel );
	{
		declare_element( TextBlock, myText );
		myText->Content = "MyText!";
		myText->Margin = 10;
		myText->Color = Color::Aquamarine;

		declare_element( Border, myBorder );
		myBorder->Content = myText;
		myBorder->Fill = new RevealBrush( Color::White );
		myBorder->Padding = 10;
		relativePanel->Add( myBorder );

		declare_element( Button, myButton );
		myButton->Content = "MyButton";
		myButton->AddDependencyProperty( "RelativePanel.Below=myBorder" );
		relativePanel->Add( myButton );
	}

	Content = relativePanel;
}