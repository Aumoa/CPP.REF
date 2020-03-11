using namespace SC;
using namespace SC::Game::UI;

namespace
{
	std::hash<std::wstring> hash;
}

DependencyObject::DependencyObject()
{

}

void DependencyObject::AddDependencyProperty( DependencyProperty property )
{
	properties[property.ClassName][property.PropertyName] = property.PropertyValue.Chars;
}

void DependencyObject::RemoveDependencyProperty( std::size_t className, std::size_t propertyName )
{
	if ( auto classIt = properties.find( className ); classIt != properties.end() )
	{
		if ( auto propertyIt = classIt->second.find( propertyName ); propertyIt != classIt->second.end() )
		{
			classIt->second.erase( propertyIt );
		}
	}
}

void DependencyObject::RemoveDependencyProperty( std::size_t className )
{
	if ( auto classIt = properties.find( className ); classIt != properties.end() )
	{
		properties.erase( classIt );
	}
}

bool DependencyObject::TryGetClassProperties( std::size_t className, NameValueMap* pNameValueMap )
{
	if ( auto it = properties.find( className ); it != properties.end() )
	{
		if ( pNameValueMap )
		{
			*pNameValueMap = it->second;
		}

		return true;
	}

	return false;
}