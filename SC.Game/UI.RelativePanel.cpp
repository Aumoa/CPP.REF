using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;

namespace
{
	std::hash<std::wstring> hash;
}

const std::size_t RelativePanel::ClassName( hash( L"RelativePanel" ) );
const std::size_t RelativePanel::LeftOf( hash( L"LeftOf" ) );
const std::size_t RelativePanel::Above( hash( L"Above" ) );
const std::size_t RelativePanel::RightOf( hash( L"RightOf" ) );
const std::size_t RelativePanel::Below( hash( L"Below" ) );
const std::size_t RelativePanel::AlignLeftWith( hash( L"AlignLeftWith" ) );
const std::size_t RelativePanel::AlignTopWith( hash( L"AlignTopWith" ) );
const std::size_t RelativePanel::AlignRightWith( hash( L"AlignRightWith" ) );
const std::size_t RelativePanel::AlignBottomWith( hash( L"AlignBottomWith" ) );

Rect<double> RelativePanel::OnUpdate( Rect<double> clientRect )
{
	using namespace std;

	if ( Width != 0 || Height != 0 )
	{
		clientRect.Right = clientRect.Left + Width;
		clientRect.Bottom = clientRect.Top + Height;
	}

	vector<Rect<double>> computedRects( Count, clientRect );
	vector<bool> computed( Count, false );

	auto content = Content.As<ContentType>();
	for ( int i = 0; i < Count; ++i )
	{
		ComputeRect( i, computedRects, computed );
	}

	return clientRect;
}

RelativePanel::RelativePanel( String name )
	: Panel( name )
{
	
}

void RelativePanel::ComputeRect( int index, std::vector<Rect<double>>& computedRects, std::vector<bool>& computed )
{
	auto item = Item[index];

	bool alignWithPanel[4] = { false };

	if ( computed[index] == false )
	{
		auto contentRect = computedRects[index];
		NameValueMap propertyMap;
		bool tryGet;
		if ( tryGet = item->TryGetClassProperties( ClassName, &propertyMap ) )
		{
			if ( auto it = propertyMap.find( Below ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Top = computedRects[indexOf].Bottom;
				}
			}

			if ( auto it = propertyMap.find( Above ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Bottom = computedRects[indexOf].Top;
				}
			}

			if ( auto it = propertyMap.find( RightOf ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Left = computedRects[indexOf].Right;
				}
			}

			if ( auto it = propertyMap.find( LeftOf ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Right = computedRects[indexOf].Left;
				}
			}

			if ( auto it = propertyMap.find( AlignLeftWith ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Left = computedRects[indexOf].Left;
				}
			}

			if ( auto it = propertyMap.find( AlignTopWith ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Top = computedRects[indexOf].Top;
				}
			}

			if ( auto it = propertyMap.find( AlignRightWith ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Right = computedRects[indexOf].Right;
				}
			}

			if ( auto it = propertyMap.find( AlignBottomWith ); it != propertyMap.end() )
			{
				int indexOf = IndexOf( String( it->second ) );
				if ( indexOf != -1 )
				{
					ComputeRect( indexOf, computedRects, computed );
					contentRect.Bottom = computedRects[indexOf].Bottom;
				}
			}
		}

		item->Update( contentRect );
		contentRect = item->ActualContentRect;

		computedRects[index] = contentRect;
		computed[index] = true;
	}
}