using namespace SC;
using namespace SC::Game::Details;

using namespace std;

TextParser::TextParser( const string& separates )
{
	sep.resize( separates.length() );
	memcpy( sep.data(), separates.data(), sizeof( char ) * separates.length() );
}

void TextParser::SetBuffer( const char* textBlock )
{
	this->textBlock = textBlock;
	seekpos = 0;
}

char TextParser::ReadChar()
{
	if ( !Eof )
	{
		_NextSeekpos();
		return textBlock[seekpos++];
	}

	return 0;
}

string TextParser::ReadString( bool removeQuotes )
{
	bool beginRead = false;

	if ( !Eof )
	{
		_NextSeekpos();
		auto begseek = seekpos;
		int endseek = 0;

		if ( textBlock[begseek] == '\"' )
		{
			for ( int i = 1; ; ++i )
			{
				if ( textBlock[begseek + i] == '\"' )
				{
					endseek = begseek + i + 1;
					seekpos = endseek;
					break;
				}
			}
		}
		else
		{
			_FindStoppos();
			if ( seekpos == -1 )
			{
				return "";
			}
			endseek = seekpos;
		}

		if ( removeQuotes )
		{
			if ( textBlock[begseek] == '\"' ) begseek += 1;
			if ( textBlock[endseek - 1] == '\"' ) endseek -= 1;
		}

		return string( textBlock + begseek, textBlock + endseek );
	}

	return "";
}

int TextParser::ReadInt()
{
	auto context = ReadString();
	stringstream ss( context );
	int parsed = 0;
	ss >> parsed;
	return parsed;
}

double TextParser::ReadDouble()
{
	auto context = ReadString();
	stringstream ss( context );
	double parsed = 0;
	ss >> parsed;
	return parsed;
}

int TextParser::Seekpos_get()
{
	return seekpos;
}

void TextParser::Seekpos_set( int value )
{
	seekpos = value;
}

bool TextParser::Eof_get()
{
	if ( seekpos == -1 ) return true;
	else return false;
}

void TextParser::_NextSeekpos()
{
	while ( !Eof )
	{
		if ( !_CheckIfSep( textBlock[seekpos] ) )
		{
			return;
		}
		else
		{
			seekpos++;
		}
	}

	seekpos = -1;
}

void TextParser::_FindStoppos()
{
	while ( !Eof )
	{
		auto ch = textBlock[seekpos];

		if ( ch == '\0' )
		{
			seekpos = -1;
			return;
		}
		else if ( _CheckIfSep( ch ) )
		{
			return;
		}
		else
		{
			seekpos++;
		}
	}

	seekpos = -1;
}

bool TextParser::_CheckIfSep( char ch )
{
	auto it = find( sep.begin(), sep.end(), ch );
	if ( it == sep.end() ) return false;
	else return true;
}