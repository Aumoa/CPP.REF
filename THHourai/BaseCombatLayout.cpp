using namespace Touhou;

using namespace SC::Game::UI;

BaseCombatLayout::BaseCombatLayout() : Element( "baseCombatLayout" )
{
	InitializeComponents();
}

void BaseCombatLayout::InitializeComponents()
{
	declare_element( RelativePanel, relativePanel );
	{
		declare_element( TextBlock, textBlock );
	}
}