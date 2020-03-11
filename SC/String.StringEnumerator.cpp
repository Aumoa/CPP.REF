using namespace SC;

String::StringEnumerator::StringEnumerator( const wchar_t* bufferPtr, int length )
	: bufferPtr( bufferPtr )
	, length( length )
{

}

bool String::StringEnumerator::MoveNext()
{
	indexOf += 1;
	if ( indexOf >= length ) return false;
	return true;
}

void String::StringEnumerator::Reset()
{
	indexOf = -1;
}

wchar_t String::StringEnumerator::Current_get()
{
	return bufferPtr[indexOf];
}