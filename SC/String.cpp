using namespace SC;

using namespace std;

String::String() : ValueType()
{
	ptr = make_shared<wstring>( L"" );
}

String::String( const string& text ) : ValueType()
{
	int len = MultiByteToWideChar( CP_ACP, 0, text.c_str(), ( int )text.length(), nullptr, 0 );
	ptr = make_shared<wstring>();
	ptr->resize( len );
	MultiByteToWideChar( CP_ACP, 0, text.c_str(), ( int )text.length(), ptr->data(), len );
}

String::String( const string_view& text ) : ValueType()
{
	int len = MultiByteToWideChar( CP_ACP, 0, text.data(), ( int )text.length(), nullptr, 0 );
	ptr = make_shared<wstring>();
	ptr->resize( len );
	MultiByteToWideChar( CP_ACP, 0, text.data(), ( int )text.length(), ptr->data(), len );
}

String::String( const char* text ) : String( string_view( text ) )
{

}

String::String( const wstring& text ) : ValueType()
{
	ptr = make_shared<wstring>( text );
}

String::String( wstring&& text ) : ValueType()
{
	ptr = make_shared<wstring>( move( text ) );
}

String::String( const wstring_view& text ) : ValueType()
{
	ptr = make_shared<wstring>( text );
}

String::String( const wchar_t* text ) : String( wstring_view( text ) )
{

}

String::String( const String& copy ) : ValueType()
{
	ptr = copy.ptr;
}

String::String( String&& mov ) : ValueType()
{
	ptr = move( mov.ptr );
}

String String::ToString()
{
	return *this;
}

bool String::Equals( object obj )
{
	if ( String str; obj.Is<String>( &str ) )
	{
		return Equals( str );
	}
	else
	{
		return false;
	}
}

bool String::Equals( String right )
{
	return ptr->compare( *right.ptr ) == 0;
}

int String::CompareTo( String right )
{
	return ptr->compare( *right.ptr );
}

RefPtr<IEnumerator<wchar_t>> String::GetEnumerator()
{
	return new StringEnumerator( Chars, ptr->length() );
}

object String::Clone()
{
	return String( wstring_view( *ptr ) );
}

int String::IndexOf( String value, int startIndex ) const
{
	auto ret = ptr->find( *value.ptr, ( int )startIndex );

	if ( ret == wstring::npos ) return -1;
	else return ( int )ret;
}

int String::IndexOf( wchar_t value, int startIndex ) const
{
	auto ret = ptr->find( value, ( int )startIndex );

	if ( ret == wstring::npos ) return -1;
	else return ( int )ret;
}

String String::Substring( int startIndex ) const
{
	return Substring( startIndex, Length - startIndex );
}

String String::Substring( int startIndex, int length ) const
{
	return ptr->substr( startIndex, length );
}

String String::Lowercase_get() const
{
	wstring text = *ptr;
	transform( text.begin(), text.end(), text.begin(),
		[]( wchar_t c ) { return std::tolower( c ); } );
	return move( text );
}

String String::Uppercase_get() const
{
	wstring text = *ptr;
	transform( text.begin(), text.end(), text.begin(),
		[]( wchar_t c ) { return std::toupper( c ); } );
	return move( text );
}

const wstring& String::C_Str() const
{
	return *ptr;
}

int String::Length_get() const
{
	return ( int )ptr->length();
}

const wchar_t* String::Chars_get() const
{
	return ptr->c_str();
}

bool String::IsEmpty_get() const
{
	return ptr->empty();
}

String String::Format( String formatText, vector<object> args )
{
	wostringstream woss;

	int indexOf = 0;

	while ( indexOf < formatText.Length )
	{
		auto formatIdx = formatText.IndexOf( L'{', indexOf );

		if ( formatIdx == -1 )
		{
			woss << formatText.Chars + indexOf;
			break;
		}
		else
		{
			woss << wstring_view( formatText.Chars + indexOf, formatIdx - indexOf );

			if ( formatText.Chars[formatIdx + 1] == L'{' )
			{
				woss << L'{';
				indexOf = formatIdx + 2;
			}
			else
			{
				formatIdx += 1;
				auto endBracket = formatText.IndexOf( L'}', formatIdx );
				auto substr = formatText.Substring( formatIdx, endBracket - formatIdx );
				int index = stoi( *substr.ptr );

				if ( index < ( int )args.size() )
				{
					auto str = args[index]->ToString();
					woss << str.Chars;
				}

				indexOf = endBracket + 1;
			}
		}
	}

	return woss.str();
}

String String::operator+( String right ) const
{
	return std::operator+( *ptr, *right.ptr );
}

String String::operator-( int count ) const
{
	return ptr->substr( 0, Length - count );
}

String& String::operator=( String right )
{
	ptr = right.ptr;
	return *this;
}

String& String::operator+=( String right )
{
	ptr = make_shared<wstring>( move( std::operator+( *ptr, *right.ptr ) ) );
	return *this;
}

bool String::operator< ( String right ) const
{
	return ptr->compare( *right.ptr ) < 0;
}

bool String::operator<=( String right ) const
{
	return ptr->compare( *right.ptr ) <= 0;
}

bool String::operator> ( String right ) const
{
	return ptr->compare( *right.ptr ) > 0;
}

bool String::operator>=( String right ) const
{
	return ptr->compare( *right.ptr ) >= 0;
}

bool String::operator==( String right ) const
{
	return *ptr == *right.ptr;
}

bool String::operator!=( String right ) const
{
	return *ptr != *right.ptr;
}

bool String::operator< ( wstring_view right ) const
{
	return ptr->compare( right ) < 0;
}

bool String::operator<=( wstring_view right ) const
{
	return ptr->compare( right ) <= 0;
}

bool String::operator> ( wstring_view right ) const
{
	return ptr->compare( right ) > 0;
}

bool String::operator>=( wstring_view right ) const
{
	return ptr->compare( right ) >= 0;
}

bool String::operator==( wstring_view right ) const
{
	return *ptr == right;
}

bool String::operator!=( wstring_view right ) const
{
	return *ptr != right;
}