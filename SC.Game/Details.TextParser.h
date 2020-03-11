#pragma once

namespace SC::Game::Details
{
	class TextParser : virtual public Object
	{
		std::vector<char> sep;

		const char* textBlock = nullptr;
		int seekpos = 0;

	public:
		TextParser( const std::string& separates = " ,{}[]()+-/*\n" );

		void SetBuffer( const char* textBlock );

		char ReadChar();
		std::string ReadString( bool removeQuotes = true );
		int ReadInt();
		double ReadDouble();

		vs_property( int, Seekpos );
		int Seekpos_get();
		void Seekpos_set( int value );
		vs_property_get( bool, Eof );
		bool Eof_get();

	private:
		void _NextSeekpos();
		void _FindStoppos();
		bool _CheckIfSep( char ch );
	};
}