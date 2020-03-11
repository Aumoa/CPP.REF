using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

Element::Element( String name )
{
	Name = name;
	Anchor = UI::Anchor::LeftTop;
}

Element::~Element()
{

}

String Element::ToString()
{
	return String::Format( "Element({0})", name );
}

bool Element::Equals( object obj )
{
	if ( RefPtr<Element> elem; obj.Is<Element>( &elem ) )
	{
		return Equals( elem );
	}
	else if ( String str; obj.Is<String>( &str ) )
	{
		return Equals( str );
	}
	else
	{
		return false;
	}
}

bool Element::Equals( RefPtr<Element> right )
{
	if ( right.IsValid )
	{
		return name.Equals( right->name );
	}
	
	return false;
}

bool Element::Equals( String right )
{
	return name.Equals( right );
}

void Element::Update( Rect<double> clientRect )
{
	clientRect.Left += margin.Left;
	clientRect.Top += margin.Top;
	clientRect.Right -= margin.Right;
	clientRect.Bottom -= margin.Bottom;

	Point<double> offset[9]
	{
		{ 0, 0 }, { 0.5, 0 }, { 1, 0 },
		{ 0, 0.5 }, { 0.5, 0.5 }, { 1, 0.5 },
		{ 0, 1 }, { 0.5, 1 }, { 1, 1 }
	};

	if ( Width != 0 )
	{
		auto right = clientRect.Right - Width;
		auto length = right - clientRect.Left;

		int idx = ( int )anchor;
		clientRect.Left += offset[idx].X * length;
		clientRect.Right = clientRect.Left + Width;
	}

	if ( Height != 0 )
	{
		auto bottom = clientRect.Bottom - Height;
		auto length = bottom - clientRect.Top;

		int idx = ( int )anchor;
		clientRect.Top += offset[idx].Y * length;
		clientRect.Bottom = clientRect.Top + Height;
	}

	clientRect = OnUpdate( clientRect );
	actualRenderingRect = clientRect;
}

void Element::Render( RefPtr<CDeviceContext>& deviceContext )
{
	OnRender( deviceContext );
}

void Element::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	switch ( args->Type )
	{
	case DispatcherEventType::MouseMove:
		OnMouseMove( args->Argument.As<MouseMoveEventArgs>() );
		break;
	case DispatcherEventType::MouseClick:
		OnMouseClick( args->Argument.As<MouseClickEventArgs>() );
		break;
	}
}

RefPtr<SC::Object> Element::Content_get()
{
	return content;
}

void Element::Content_set( RefPtr<SC::Object> value )
{
	content = value;
	ContentChanged( nullptr, value );
}

double Element::Width_get()
{
	return width;
}

void Element::Width_set( double value )
{
	width = value;
	ContentSizing( nullptr, { width, height } );
}

double Element::Height_get()
{
	return height;
}

void Element::Height_set( double value )
{
	height = value;
	ContentSizing( nullptr, { width, height } );
}

Rect<double> Element::ActualContentRect_get()
{
	return actualRenderingRect;
}

Thickness Element::Margin_get()
{
	return margin;
}

void Element::Margin_set( Thickness value )
{
	margin = value;
}

String Element::Name_get()
{
	return name;
}

void Element::Name_set( String value )
{
	name = value;
}

Anchor Element::Anchor_get()
{
	return anchor;
}

void Element::Anchor_set( UI::Anchor value )
{
	anchor = value;
}

void Element::OnMouseMove( MouseMoveEventArgs args )
{
	auto cursor = ( Point<double> )args.AbsCursorPos;

	if ( cursor.InRect( actualRenderingRect ) )
	{
		if ( hovered == false )
		{
			hovered = true;
			MouseEnterLeave( nullptr, true );
		}
	}
	else
	{
		if ( hovered )
		{
			hovered = false;
			MouseEnterLeave( nullptr, false );
		}
	}

	MouseMove( nullptr, cursor );
}

void Element::OnMouseClick( MouseClickEventArgs args )
{
	MouseClick( nullptr, args );
}