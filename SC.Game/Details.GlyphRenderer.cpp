using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;
using namespace SC::Game::UI;

using namespace std;

GlyphRenderer::GlyphRenderer( GlyphBuffer* glyphBuffer ) : Object()
    , glyphBuffer( glyphBuffer )
{
    fillBrush = new SolidColorBrush( Drawing::Color::White );
}

ULONG STDMETHODCALLTYPE GlyphRenderer::AddRef()
{
	return 1;
}

ULONG STDMETHODCALLTYPE GlyphRenderer::Release()
{
	return 1;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::QueryInterface( REFIID riid, void** ppvObject )
{
	if ( riid == __uuidof( IUnknown ) )
	{
		*ppvObject = ( IUnknown* )this;
        return S_OK;
	}
	else if ( riid == __uuidof( IDWriteTextRenderer ) )
	{
		*ppvObject = ( IDWriteTextRenderer* )this;
        return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::DrawGlyphRun( void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, DWRITE_GLYPH_RUN const* glyphRun, DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, IUnknown* clientDrawingEffect )
{
    auto deviceContext = ( CDeviceContext* )clientDrawingContext;
    auto pCommandList = deviceContext->pCommandList.Get();

    if ( auto slot = deviceContext->Slot["Brush"]; slot != -1 )
    {
        auto brush = fillBrush.Get();
        if ( clientDrawingEffect ) brush = dynamic_cast< RichTextColorUnknown* >( clientDrawingEffect )->solidColorBrush.Get();

        brush->Opacity = Opacity;
        pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, brush->brushConstants->VirtualAddress );
    }

    if ( auto slot = deviceContext->Slot["RenderType"]; slot != -1 )
    {
        pCommandList->SetGraphicsRoot32BitConstant( ( UINT )slot, 0, 0 );
    }

    glyphBuffer->DrawGlyphRun( deviceContext, baselineOriginX, baselineOriginY, glyphRun );

    return S_OK;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::DrawUnderline( void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_UNDERLINE const* underline, IUnknown* clientDrawingEffect )
{
    auto deviceContext = ( CDeviceContext* )clientDrawingContext;
    auto pCommandList = deviceContext->pCommandList.Get();

    tag_Rect output;

    output.left = baselineOriginX;
    output.top = baselineOriginY + underline->offset - underline->thickness;
    output.width = underline->width;
    output.height = underline->thickness;

    tag_ShaderInfo shaderInfo;
    shaderInfo.output = output;

    if ( auto slot = deviceContext->Slot["Brush"]; slot != -1 )
    {
        auto brush = fillBrush.Get();
        if ( clientDrawingEffect ) brush = dynamic_cast< RichTextColorUnknown* >( clientDrawingEffect )->solidColorBrush.Get();

        brush->Opacity = Opacity;
        pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, brush->brushConstants->VirtualAddress );
    }

    if ( auto slot = deviceContext->Slot["RenderType"]; slot != -1 )
    {
        pCommandList->SetGraphicsRoot32BitConstant( ( UINT )slot, 1, 0 );
    }

    deviceContext->DispatchShaderInfo( 1, &shaderInfo );

    return S_OK;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::DrawStrikethrough( void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_STRIKETHROUGH const* strikethrough, IUnknown* clientDrawingEffect )
{
    auto deviceContext = ( CDeviceContext* )clientDrawingContext;
    auto pCommandList = deviceContext->pCommandList.Get();

    tag_Rect output;

    output.left = baselineOriginX;
    output.top = baselineOriginY + strikethrough->offset - strikethrough->thickness;
    output.width = strikethrough->width;
    output.height = strikethrough->thickness;

    tag_ShaderInfo shaderInfo;
    shaderInfo.output = output;

    if ( auto slot = deviceContext->Slot["Brush"]; slot != -1 )
    {
        auto brush = fillBrush.Get();
        if ( clientDrawingEffect ) brush = dynamic_cast< RichTextColorUnknown* >( clientDrawingEffect )->solidColorBrush.Get();

        brush->Opacity = Opacity;
        pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, brush->brushConstants->VirtualAddress );
    }

    if ( auto slot = deviceContext->Slot["RenderType"]; slot != -1 )
    {
        pCommandList->SetGraphicsRoot32BitConstant( ( UINT )slot, 1, 0 );
    }

    deviceContext->DispatchShaderInfo( 1, &shaderInfo );
    return S_OK;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::DrawInlineObject( void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect )
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::IsPixelSnappingDisabled( void* clientDrawingContext, BOOL* isDisabled )
{
    *isDisabled = true;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::GetCurrentTransform( void* clientDrawingContext, DWRITE_MATRIX* transform )
{
    auto dc = ( ID2D1DeviceContext* )clientDrawingContext;
    dc->GetTransform( ( D2D1_MATRIX_3X2_F* )transform );

    return S_OK;
}

HRESULT STDMETHODCALLTYPE GlyphRenderer::GetPixelsPerDip( void* clientDrawingContext, FLOAT* pixelsPerDip )
{
    auto dc = ( ID2D1DeviceContext* )clientDrawingContext;
    FLOAT dpiX, dpiY;
    dc->GetDpi( &dpiX, &dpiY );
    *pixelsPerDip = dpiX / 96.0f;

    return S_OK;
}

Drawing::Color GlyphRenderer::FillColor_get()
{
    return fillBrush->SolidColor;
}

void GlyphRenderer::FillColor_set( Drawing::Color value )
{
    fillBrush->SolidColor = value;
}