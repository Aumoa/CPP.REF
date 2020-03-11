using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

Button::Button( String name )
	: Element( name )
{
	MouseEnterLeave += EventHandler<bool>( this, &Button::OnMouseEnterLeave );
	ContentChanged += EventHandler<object>( this, &Button::OnContentChanged );
	MouseClick += EventHandler<MouseClickEventArgs>( this, &Button::OnMouseClick );

	border = new Border( "Button.border", 1.0 );
	border->Fill = new RevealBrush( Drawing::Color::LightGray );

	Width = 100;
	Height = 100;

	buttonText = new TextBlock( "Button.textBlock" );
	buttonText->Alignment = TextAlignment::Center;
	buttonText->VerticalAlignment = TextVerticalAlignment::Center;

	Content = "Button";
}

Rect<double> Button::OnUpdate( Rect<double> clientRect )
{
	border->Update( clientRect );

	auto rect2 = clientRect;
	if ( push )
	{
		rect2.Top += 1.0;
		rect2.Bottom += 1.0;
	}

	if ( innerElement.IsValid )
	{
		innerElement->Update( rect2 );
	}
	else
	{
		buttonText->Width = clientRect.Width;
		buttonText->Height = clientRect.Height;
		buttonText->Update( rect2 );
	}

	return clientRect;
}

void Button::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	if ( innerElement.IsValid ) innerElement->Render( deviceContext );
	else buttonText->Render( deviceContext );
	border->Render( deviceContext );
}

void Button::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	border->ProcessEvent( args );
	if ( innerElement.IsValid ) innerElement->ProcessEvent( args );
	else buttonText->ProcessEvent( args );
	return Element::ProcessEvent( args );
}

void Button::OnMouseEnterLeave( object sender, bool hover )
{
	this->hover = hover;
	if ( hover )
	{
		border->Fill.As<RevealBrush>()->MinOpacity = 0.5;
	}
	else
	{
		border->Fill.As<RevealBrush>()->MinOpacity = 0.0;
	}
}

void Button::OnContentChanged( object sender, object content )
{
	if ( RefPtr<Element> item; content.Is<Element>( &item ) )
	{
		innerElement = item;
	}
	else
	{
		buttonText->Content = content;
		innerElement = nullptr;
	}
}

void Button::OnMouseClick( object sender, MouseClickEventArgs args )
{
	if ( args.Button == MouseButtonType::LeftButton )
	{
		if ( args.IsDown )
		{
			if ( hover )
			{
				push = true;
			}
		}
		else
		{
			if ( hover )
			{
				Clicked( this );
			}
			push = false;
		}
	}
}