using namespace SC;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

namespace
{
	std::hash<std::wstring> hash_;
}

const std::size_t Panel::ClassName( hash_( L"Panel" ) );
const std::size_t Panel::ZOrder( hash_( L"ZOrder" ) );

using namespace std;

Panel::Panel( String name )
	: Element( name )
{
	contents = make_shared<ContentType::element_type>();
	Content = contents;
}

void Panel::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	std::map<int, std::vector<Element*>> ordered_elements;

	for ( int i = 0; i < Count; ++i )
	{
		DependencyObject::NameValueMap valueMap;
		int order = 0;
		if ( contents->at( i )->TryGetClassProperties( ClassName, &valueMap ) )
		{
			if ( auto it = valueMap.find( ZOrder ); it != valueMap.end() )
			{
				order = Int32::Parse( it->second.c_str() );
			}
		}
		ordered_elements[order].push_back( contents->at( i ).Get() );
	}

	for ( auto& i : ordered_elements )
	{
		for ( auto& j : i.second )
		{
			j->Render( deviceContext );
		}
	}
}

RefPtr<IEnumerator<RefPtr<Element>>> Panel::GetEnumerator()
{
	return new Enumerator( contents->begin(), contents->end() );
}

int Panel::Add( RefPtr<Element> value )
{
	int c = Count;
	contents->push_back( value );
	return c;
}

void Panel::Clear()
{
	contents->clear();
}

bool Panel::Contains( RefPtr<Element> value )
{
	auto it = std::find_if( contents->begin(), contents->end(), [&]( RefPtr<Element> arg0 ) -> bool
		{
			return arg0->Equals( value );
		}
	);

	return it != contents->end();
}

void Panel::CopyTo( RefPtr<Element>* array, int index )
{
	for ( int i = index; i < Count; ++i )
	{
		array[i - index] = contents->at( i );
	}
}

int Panel::IndexOf( RefPtr<Element> value )
{
	auto it = std::find_if( contents->begin(), contents->end(), [&]( RefPtr<Element> arg0 ) -> bool
		{
			return arg0->Equals( value );
		}
	);

	return it - contents->begin();
}

void Panel::Insert( int index, RefPtr<Element> value )
{
	contents->insert( contents->begin() + index, value );
}

void Panel::Remove( RefPtr<Element> value )
{
	if ( auto index = IndexOf( value ); index != -1 )
	{
		contents->erase( contents->begin() + index );
	}
}

void Panel::RemoveAt( int index )
{
	contents->erase( contents->begin() + index );
}

void Panel::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	std::map<int, std::vector<Element*>> ordered_elements;

	for ( int i = 0; i < Count; ++i )
	{
		DependencyObject::NameValueMap valueMap;
		int order = 0;
		if ( contents->at( i )->TryGetClassProperties( ClassName, &valueMap ) )
		{
			if ( auto it = valueMap.find( ZOrder ); it != valueMap.end() )
			{
				order = Int32::Parse( it->second.c_str() );
			}
		}
		ordered_elements[order].push_back( contents->at( i ).Get() );
	}

	for ( auto i = ordered_elements.rbegin(); i != ordered_elements.rend(); ++i )
	{
		for ( auto j = i->second.rbegin(); j != i->second.rend(); ++j )
		{
			( *j )->ProcessEvent( args );
		}
	}
}

int Panel::IndexOf( String name )
{
	auto it = std::find_if( contents->begin(), contents->end(), [&]( RefPtr<Element> arg0 ) -> bool
		{
			return arg0->Equals( name );
		}
	);

	auto index = it - contents->begin();
	if ( index >= ( ptrdiff_t )contents->size() ) return -1;
	return index;
}

int Panel::Count_get()
{
	return ( int )contents->size();
}

bool Panel::IsFixedSize_get()
{
	return false;
}

RefPtr<Element> Panel::Item_get( int index )
{
	return contents->at( index );
}

void Panel::Item_set( int index, RefPtr<Element> value )
{
	contents->at( index ) = value;
}