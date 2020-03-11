#pragma once

namespace SC::Game::Details
{
	class GlyphBuffer : virtual public Object
	{
		static constexpr const DXGI_FORMAT Format = DXGI_FORMAT_A8_UNORM;
		static constexpr const float padding = 2.0f;

		friend class UI::TextFormat;

		struct tag_Range
		{
			int x;
		};

	public:
		struct tag_GlyphInfo
		{
			uint16 glyphIndex;
			IDWriteFontFace* pFontFace;
			float offsetX;
			float width;

			bool operator<( const tag_GlyphInfo& right ) const
			{
				if ( glyphIndex < right.glyphIndex ) return true;
				else if ( glyphIndex > right.glyphIndex ) return false;
				else if ( pFontFace < right.pFontFace ) return true;
				else return false;
			}
		};

	private:
		ComPtr<IDWriteTextFormat> pTextFormat;
		ComPtr<IDWriteFont> pFont;
		ComPtr<ID3D12CommandAllocator> pCommandAllocator;
		RefPtr<CDeviceContext> deviceContext;
		ComPtr<ID2D1SolidColorBrush> pAlpha1Brush;
		float fontEmSize = 0;

		uint32 maxWidth = 0;
		float maxHeight = 0;
		uint32 maxHeightAligned = 0;
		float ascent = 0;
		float descent = 0;

		ComPtr<ID3D12Resource> pGlyphTexture;
		ComPtr<ID3D11Resource> pGlyphTextureInterop;
		ComPtr<ID2D1Bitmap1> pGlyphBitmap;
		RefPtr<CShaderResourceView> pShaderResourceView;
		float lastGlyphOffset = 0;
		
		std::map<tag_GlyphInfo, float> allocated;
		std::set<tag_GlyphInfo> locked;
		float lockedWidth = 0;
		std::set<tag_GlyphInfo> added;
		float appendWidth = 0;

		uint64 lastPending = 0;
		std::mutex glyphLocking;

	public:
		GlyphBuffer( String fontFamilyName, float fontSize );
		~GlyphBuffer() override;

		void Restart();
		void PushGlyphRun( const DWRITE_GLYPH_RUN* glyphRun );
		void LockGlyphs();
		void DrawGlyphRun( CDeviceContext* clientDrawingContext, float baselineX, float baselineY, const DWRITE_GLYPH_RUN* glyphRun );

		float designUnitsPerEm = 0;
		float designSize = 0;

	private:
		void Expand();
	};

	inline bool operator<( uint16 left, const GlyphBuffer::tag_GlyphInfo& right )
	{
		return left < right.glyphIndex;
	}
}