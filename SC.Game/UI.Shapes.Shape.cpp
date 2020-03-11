using namespace SC;
using namespace SC::Game::UI;
using namespace SC::Game::UI::Shapes;

Shape::Shape( String name )
	: Element( name )
{
	Fill = new SolidColorBrush( Drawing::Color::Black );
}

RefPtr<Brush> Shape::Fill_get()
{
	return brush;
}

void Shape::Fill_set( RefPtr<Brush> value )
{
	brush = value;
}