using namespace SC;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

Page::Page()
{

}

void Page::OnNavigatedTo( RefPtr<NavigationEventArgs> args )
{

}

void Page::OnNavigatedFrom( RefPtr<NavigationEventArgs> args )
{

}

void Page::Update( Drawing::Rect<double> clientRect )
{
	if ( content.IsValid )
	{
		content->Update( clientRect );
	}
}

void Page::Render( RefPtr<CDeviceContext>& deviceContext )
{
	if ( content.IsValid )
	{
		content->Render( deviceContext );
	}
}

void Page::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	if ( content.IsValid )
	{
		content->ProcessEvent( args );
	}
}

RefPtr<Element> Page::Content_get()
{
	return content;
}

void Page::Content_set( RefPtr<Element> value )
{
	content = value;
}