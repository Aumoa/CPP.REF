using namespace SC;
using namespace SC::Game::UI;

Panel::Enumerator::Enumerator( iterator beginit, iterator endit )
	: beginit( beginit )
	, endit( endit )
{
	Reset();
}

bool Panel::Enumerator::MoveNext()
{
	if ( started == false )
	{
		started = true;
		if ( ( current = beginit ) == endit )
		{
			return false;
		}
		return true;
	}
	else
	{
		if ( ++current == endit )
		{
			return false;
		}
		return true;
	}
}

void Panel::Enumerator::Reset()
{
	started = false;
}

RefPtr<Element> Panel::Enumerator::Current_get()
{
	return *current;
}