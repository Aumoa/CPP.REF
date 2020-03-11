using namespace SC;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

TextFormat::TextFormat( String fontFamilyName, double fontSize ) : Object()
	, fontFamilyName( fontFamilyName )
	, fontSize( fontSize )
{
	glyphBuffer = new GlyphBuffer( fontFamilyName, ( float )fontSize );
	pTextFormat = glyphBuffer->pTextFormat.Get();
}

TextFormat::~TextFormat()
{

}