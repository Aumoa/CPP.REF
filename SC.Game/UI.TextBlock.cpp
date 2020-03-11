using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;
using namespace SC::Game::Details;
using namespace SC::Drawing;

using namespace std;

Rect<double> TextBlock::OnUpdate( Rect<double> clientRect )
{
	if ( pLayout )
	{
		HR( pLayout->SetMaxWidth( ( float )clientRect.Width ) );
		HR( pLayout->SetMaxHeight( ( float )clientRect.Height ) );

		if ( Width == 0 && Height == 0 && autoSizing )
		{
			DWRITE_TEXT_METRICS metrics;
			HR( pLayout->GetMetrics( &metrics ) );

			auto actualWidth = ( double )metrics.width;
			auto actualHeight = ( double )metrics.height;

			Point<double> offset[9]
			{
				{ 0, 0 }, { 0.5, 0 }, { 1, 0 },
				{ 0, 0.5 }, { 0.5, 0.5 }, { 1, 0.5 },
				{ 0, 1 }, { 0.5, 1 }, { 1, 1 }
			};

			auto right = clientRect.Right - actualWidth;
			auto length = right - clientRect.Left;

			int idx = ( int )Anchor;
			clientRect.Left += offset[idx].X * length;
			clientRect.Right = clientRect.Left + actualWidth;

			auto bottom = clientRect.Bottom - actualHeight;
			length = bottom - clientRect.Top;

			idx = ( int )Anchor;
			clientRect.Top += offset[idx].Y * length;
			clientRect.Bottom = clientRect.Top + actualHeight;
		}

		HR( pLayout->SetMaxWidth( ( float )clientRect.Width ) );
		HR( pLayout->SetMaxHeight( ( float )clientRect.Height ) );
	}

	return clientRect;
}

void TextBlock::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	if ( pLayout )
	{
		auto rect = ( Drawing::Rect<float> )ActualContentRect;
		pLayout->Draw( deviceContext.Get(), glyphRenderer.Get(), rect.Left, rect.Top );
	}
}

TextBlock::TextBlock( String name ) : Element( name )
{
	ContentChanged += { this, & TextBlock::OnContentChanged };

	textFormat = GlobalVar.defaultTextFormat;

	OnFormatChanged();
}

TextBlock::~TextBlock()
{

}

RefPtr<TextFormat> TextBlock::Format_get()
{
	return textFormat;
}

void TextBlock::Format_set( RefPtr<TextFormat> value )
{
	textFormat = value;
	OnFormatChanged();
}

TextAlignment TextBlock::Alignment_get()
{
	return textAlignment;
}

void TextBlock::Alignment_set( TextAlignment value )
{
	DWRITE_TEXT_ALIGNMENT alignments[]
	{
		DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_TEXT_ALIGNMENT_TRAILING,
		DWRITE_TEXT_ALIGNMENT_JUSTIFIED
	};

	if ( pLayout )
	{
		HR( pLayout->SetTextAlignment( alignments[( int )value] ) );
	}
	textAlignment = value;
}

TextVerticalAlignment TextBlock::VerticalAlignment_get()
{
	return verticalAlignment;
}

void TextBlock::VerticalAlignment_set( TextVerticalAlignment value )
{
	DWRITE_PARAGRAPH_ALIGNMENT alignments[]
	{
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_FAR
	};

	if ( pLayout )
	{
		HR( pLayout->SetParagraphAlignment( alignments[( int )value] ) );
	}
	verticalAlignment = value;
}

bool TextBlock::AutoSizing_get()
{
	return autoSizing;
}

void TextBlock::AutoSizing_set( bool value )
{
	autoSizing = value;
}

Color TextBlock::Color_get()
{
	return glyphRenderer->FillColor;
}

void TextBlock::Color_set( Drawing::Color value )
{
	glyphRenderer->FillColor = value;
}

double TextBlock::Opacity_get()
{
	return glyphRenderer->Opacity;
}

void TextBlock::Opacity_set( double value )
{
	glyphRenderer->Opacity = value;
}

bool TextBlock::IsRichText_get()
{
	return isRichText;
}

void TextBlock::IsRichText_set( bool value )
{
	isRichText = value;
}

double TextBlock::OutlineThickness_get()
{
	//return glyphRenderer.TryAs<GlyphRenderer>()->OutlineThickness;
	return 0;
}

void TextBlock::OutlineThickness_set( double value )
{
	//glyphRenderer.TryAs<GlyphRenderer>()->OutlineThickness = value;
}

Drawing::Color TextBlock::OutlineColor_get()
{
	//auto color = glyphRenderer.TryAs<GlyphRenderer>()->OutlineColor;
	//return Drawing::Color( color.r, color.g, color.b, color.a );
	return Drawing::Color::Black;
}

void TextBlock::OutlineColor_set( Drawing::Color value )
{
	//glyphRenderer.TryAs<GlyphRenderer>()->OutlineColor = D2D1::ColorF( value.R, value.G, value.B, value.A );
}

void TextBlock::OnFormatChanged()
{
	if ( glyphRenderer )
	{
		glyphRenderer->glyphBuffer = textFormat->glyphBuffer.Get();
	}
	else
	{
		glyphRenderer = new GlyphRenderer( textFormat->glyphBuffer.Get() );
	}
	OnContentChanged( nullptr, Content );
}

void TextBlock::OnContentChanged( object sender, object content )
{
	if ( content.IsValid )
	{
		auto str = content->ToString();
		ParseText( str );

		Alignment_set( Alignment );
		VerticalAlignment_set( VerticalAlignment );
	}
}

void TextBlock::ParseText( String original )
{
	wostringstream wss;

	vector<pair<DWRITE_TEXT_RANGE, Drawing::Color>> colorContexts;
	stack<pair<int, Drawing::Color>> colorStack;

	vector<DWRITE_TEXT_RANGE> boldContexts;
	stack<int> boldStack;

	vector<DWRITE_TEXT_RANGE> underlineContexts;
	stack<int> underlineStack;

	vector<DWRITE_TEXT_RANGE> throughContexts;
	stack<int> throughStack;

	vector<DWRITE_TEXT_RANGE> italicContexts;
	stack<int> italicStack;

	int i = 0;
	while ( i < original.Length )
	{
		auto indexOf = original.IndexOf( L'<', i );

		// 문자열에 특수 컨텍스트가 없습니다.
		if ( indexOf == -1 )
		{
			wss << original.Chars + i;
			break;
		}
		else
		{
			// 토큰 시작 전 문자열을 기록합니다.
			wss << wstring_view( original.Chars + i, indexOf - i );

			// 토큰 종료 지점을 검색합니다.
			auto endOf = original.IndexOf( L'>', indexOf );

			// 문자열 파싱 에러입니다.
			if ( endOf == -1 )
			{
				wss << original.Chars + i;
				return;
			}
			else
			{
				auto token = original.Substring( indexOf + 1, endOf - indexOf - 1 );

				// 각 토큰에 대한 행동을 수행합니다.
				ForEachTokens( token, [&]( String key, String value ) -> bool
					{
						if ( key.Chars[0] != L'/' )  // Close seq
						{
							stack<int>* stack = nullptr;

							if ( key == L"b"sv ) { stack = &boldStack; }
							else if ( key == L"u"sv ) { stack = &underlineStack; }
							else if ( key == L"t"sv ) { stack = &throughStack; }
							else if ( key == L"i"sv ) { stack = &italicStack; }
							else if ( key == L"color"sv )
							{
								auto pos = ( int )wss.tellp();
								auto color = Color::TryParse( value );
								colorStack.push( { pos, color } );
								return true;
							}
							else { return false; }

							stack->push( ( int )wss.tellp() );
							return true;
						}
						else
						{
							stack<int>* stack = nullptr;
							vector<DWRITE_TEXT_RANGE>* contexts = nullptr;

							if ( key == L"/b"sv )
							{
								stack = &boldStack;
								contexts = &boldContexts;
							}
							else if ( key == L"/u"sv )
							{
								stack = &underlineStack;
								contexts = &underlineContexts;
							}
							else if ( key == L"/t"sv )
							{
								stack = &throughStack;
								contexts = &throughContexts;
							}
							else if ( key == L"/color"sv )
							{
								auto c = colorStack.top();
								colorStack.pop();

								colorContexts.push_back( { { ( uint32 )c.first, ( uint32 )wss.tellp() - ( uint32 )c.first }, c.second } );
								return true;
							}
							else
							{
								return false;
							}

							auto pos = stack->top();
							stack->pop();
							contexts->push_back( DWRITE_TEXT_RANGE{ ( uint32 )pos, ( uint32 )wss.tellp() - ( uint32 )pos } );
							return true;
						}
					}
				);

				i = endOf + 1;
			}
		}
	}

	auto pDWriteFactory = GlobalVar.factory->pDWriteFactory.Get();
	auto str = wss.str();

	HR( pDWriteFactory->CreateTextLayout( str.c_str(), ( UINT32 )str.length(), textFormat->pTextFormat, 0, 0, &pLayout ) );

	richColors.resize( colorContexts.size() );
	for ( size_t i = 0; i < colorContexts.size(); ++i )
	{
		auto cont = colorContexts[i];
		HR( pLayout->SetDrawingEffect( new RichTextColorUnknown( cont.second ), cont.first ) );
	}

	for ( size_t i = 0; i < boldContexts.size(); ++i )
	{
		auto cont = boldContexts[i];
		HR( pLayout->SetFontWeight( DWRITE_FONT_WEIGHT_BOLD, cont ) );
	}

	for ( size_t i = 0; i < underlineContexts.size(); ++i )
	{
		auto cont = underlineContexts[i];
		HR( pLayout->SetUnderline( TRUE, cont ) );
	}

	for ( size_t i = 0; i < italicContexts.size(); ++i )
	{
		auto cont = italicContexts[i];
		HR( pLayout->SetFontStyle( DWRITE_FONT_STYLE_ITALIC, cont ) );
	}

	for ( size_t i = 0; i < throughContexts.size(); ++i )
	{
		auto cont = throughContexts[i];
		HR( pLayout->SetStrikethrough( TRUE, cont ) );
	}
}

void TextBlock::ForEachTokens( String token, function<bool( String key, String value )> lambda )
{
	auto sep = token.IndexOf( L'=' );
	if ( sep == -1 )
	{
		lambda( token, "" );
	}
	else
	{
		lambda(
			token.Substring( 0, sep ),
			token.Substring( sep + 1, token.Length - sep - 1 )
		);
	}
}