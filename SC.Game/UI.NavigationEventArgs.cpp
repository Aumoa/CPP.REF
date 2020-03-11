using namespace SC;
using namespace SC::Game::UI;

NavigationEventArgs::NavigationEventArgs( RefPtr<Page> pageFrom, RefPtr<Page> pageTo )
	: pageFrom( pageFrom )
	, pageTo( pageTo )
{

}

RefPtr<Page> NavigationEventArgs::PageFrom_get()
{
	return pageFrom;
}

RefPtr<Page> NavigationEventArgs::PageTo_get()
{
	return pageTo;
}