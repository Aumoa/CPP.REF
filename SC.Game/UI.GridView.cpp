using namespace SC;
using namespace SC::Game::UI;
using namespace SC::Drawing;

Rect<double> GridView::OnUpdate( Rect<double> clientRect )
{
	Rect<double> rect;
	rect.Left = clientRect.Left;
	rect.Top = clientRect.Top;
	rect.Right = rect.Left + gridWidth;
	rect.Bottom = rect.Top + gridHeight;

	for ( int i = 0; i < Count; ++i )
	{
		auto item = Item[i];

		if ( rect.Right > clientRect.Right )
		{
			rect.Left = clientRect.Left;
			rect.Right = rect.Left + gridWidth;
			rect.Top += gridHeight;
			rect.Bottom += gridHeight;
		}

		item->Update( rect );

		rect.Left += gridWidth;
		rect.Right += gridWidth;
	}

	return clientRect;
}

GridView::GridView( String name )
	: ViewBase( name )
{
	gridWidth = 100;
	gridHeight = 100;
}

double GridView::GridWidth_get()
{
	return gridWidth;
}

void GridView::GridWidth_set( double value )
{
	gridWidth = value;
}

double GridView::GridHeight_get()
{
	return gridHeight;
}

void GridView::GridHeight_set( double value )
{
	gridHeight = value;
}