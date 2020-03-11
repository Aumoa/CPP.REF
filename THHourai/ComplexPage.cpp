using namespace Touhou;

using namespace SC::Game::UI;

ComplexPage::ComplexPage() : BasePage()
{

}

void ComplexPage::Add( RefPtr<Element> page )
{
	fixedPanel->Add( page );
}

void ComplexPage::Remove( RefPtr<Element> page )
{
	fixedPanel->Remove( page );
}

void ComplexPage::InitializeComponents()
{
	declare_element( FixedPanel, fixedPanel );
	Content = fixedPanel;
}