using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;

namespace
{
	std::hash<std::wstring> hash;
}

const std::size_t FixedPanel::ClassName( hash( L"FixedPanel" ) );
const std::size_t FixedPanel::Left( hash( L"Left" ) );
const std::size_t FixedPanel::Top( hash( L"Top" ) );
const std::size_t FixedPanel::Right( hash( L"Right" ) );
const std::size_t FixedPanel::Bottom( hash( L"Bottom" ) );

using namespace std;

Rect<double> FixedPanel::OnUpdate( Rect<double> clientRect )
{
	auto content = Content.As<ContentType>();
	auto iterator = GetEnumerator();

	for ( auto element : iterator )
	{
		auto contentRect2 = clientRect;

		NameValueMap propertyMap;
		if ( element->TryGetClassProperties( ClassName, &propertyMap ) )
		{
			if ( auto it = propertyMap.find( Left ); it != propertyMap.end() )
			{
				contentRect2.Left = Double::Parse( it->second.c_str() );
			}

			if ( auto it = propertyMap.find( Top ); it != propertyMap.end() )
			{
				contentRect2.Top = Double::Parse( it->second.c_str() );
			}

			if ( auto it = propertyMap.find( Right ); it != propertyMap.end() )
			{
				contentRect2.Right = Double::Parse( it->second.c_str() );
			}

			if ( auto it = propertyMap.find( Bottom ); it != propertyMap.end() )
			{
				contentRect2.Bottom = Double::Parse( it->second.c_str() );
			}
		}
		element->Update( contentRect2 );
	}

	return clientRect;
}

FixedPanel::FixedPanel( String name )
	: Panel( name )
{

}