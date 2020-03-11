using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

Rect<double> Canvas::OnUpdate( Rect<double> clientRect )
{
	if ( nextPage )
	{
		page = nextPage;
		nextPage = nullptr;
	}

	if ( page.IsValid )
	{
		page->Update( clientRect );
	}

	return clientRect;
}

void Canvas::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	if ( page.IsValid )
	{
		page->Render( deviceContext );
	}
}

Canvas::Canvas( String name )
	: Element( name )
{

}

void Canvas::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	if ( page.IsValid )
	{
		page->ProcessEvent( args );
	}

	return Element::ProcessEvent( args );
}

void Canvas::Navigate( RefPtr<Page> page )
{
	RefPtr eventArgs = new NavigationEventArgs( this->page, page );

	if ( this->page.IsValid )
	{
		this->page->OnNavigatedFrom( eventArgs );
	}

	nextPage = page;
	if ( page )
		page->OnNavigatedTo( eventArgs );
}