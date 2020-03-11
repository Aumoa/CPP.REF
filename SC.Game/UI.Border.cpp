using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

using namespace std;

Rect<double> Border::OnUpdate( Rect<double> clientRect )
{
	// Align in pixels.
	clientRect.Left = ( double )( int )clientRect.Left;
	clientRect.Top = ( double )( int )clientRect.Top;
	clientRect.Right = ( double )( int )( clientRect.Right + 0.5 );
	clientRect.Bottom = ( double )( int )( clientRect.Bottom + 0.5 );

	auto contentRect = clientRect;
	contentRect.Left += padding.Left;
	contentRect.Top += padding.Top;
	contentRect.Right -= padding.Right;
	contentRect.Bottom -= padding.Bottom;

	if ( RefPtr<Element> item; Content.Is<Element>( &item ) )
	{
		item->Update( contentRect );
		contentRect = item->ActualContentRect;
	}

	clientRect.Left = ( double )( int )( contentRect.Left - padding.Left );
	clientRect.Top = ( double )( int )( contentRect.Top - padding.Top );
	clientRect.Right = ( double )( int )( contentRect.Right + padding.Right + 0.5 );
	clientRect.Bottom = ( double )( int )( contentRect.Bottom + padding.Bottom + 0.5 );

	return clientRect;
}

void Border::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	auto contentRect = ( Rect<float> )ActualContentRect;
	auto pCommandList = deviceContext->pCommandList.Get();
	float strokeWidth = ( float )this->strokeWidth;

	if ( strokeWidth >= 0.01f )
	{
		if ( auto slot = deviceContext->Slot["Brush"]; slot != -1 )
		{
			pCommandList->SetGraphicsRootConstantBufferView( slot, brush->brushConstants->VirtualAddress );
		}

		if ( auto slot = deviceContext->Slot["RenderType"]; slot != -1 )
		{
			pCommandList->SetGraphicsRoot32BitConstant( ( UINT )slot, 1, 0 );
		}

		tag_ShaderInfo shaderInfos[4];

		/* top line */
		shaderInfos[0].output.left = contentRect.Left;
		shaderInfos[0].output.top = contentRect.Top;
		shaderInfos[0].output.width = contentRect.Width;
		shaderInfos[0].output.height = strokeWidth;

		/* left line */
		shaderInfos[1].output.left = contentRect.Left;
		shaderInfos[1].output.top = contentRect.Top;
		shaderInfos[1].output.width = strokeWidth;
		shaderInfos[1].output.height = contentRect.Height;

		/* bottom line */
		shaderInfos[2].output.left = contentRect.Left;
		shaderInfos[2].output.top = contentRect.Bottom - strokeWidth;
		shaderInfos[2].output.width = contentRect.Width;
		shaderInfos[2].output.height = strokeWidth;

		/* right line */
		shaderInfos[3].output.left = contentRect.Right - strokeWidth;
		shaderInfos[3].output.top = contentRect.Top;
		shaderInfos[3].output.width = strokeWidth;
		shaderInfos[3].output.height = contentRect.Height;

		deviceContext->DispatchShaderInfo( 4, shaderInfos );
	}

	if ( RefPtr<Element> item; Content.Is<Element>( &item ) )
	{
		item->Render( deviceContext );
	}
}

Border::~Border()
{

}

Border::Border( String name, double borderThickness, Thickness padding ) : Element( name )
	, strokeWidth( borderThickness )
	, padding( padding )
{
	brush = new SolidColorBrush( Color::Black );
}

void Border::ProcessEvent( RefPtr<DispatcherEventArgs> args )
{
	if ( RefPtr<Element> item; Content.Is<Element>( &item ) )
	{
		item->ProcessEvent( args );
	}

	return Element::ProcessEvent( args );
}

double Border::StrokeWidth_get()
{
	return strokeWidth;
}

void Border::StrokeWidth_set( double value )
{
	strokeWidth = value;
}

double Border::RadiusX_get()
{
	return radiusX;
}

void Border::RadiusX_set( double value )
{
	radiusX = value;
}

double Border::RadiusY_get()
{
	return radiusY;
}

void Border::RadiusY_set( double value )
{
	radiusY = value;
}

Thickness Border::Padding_get()
{
	return padding;
}

void Border::Padding_set( Thickness value )
{
	padding = value;
}

RefPtr<Brush> Border::Fill_get()
{
	return brush;
}

void Border::Fill_set( RefPtr<Brush> value )
{
	brush = value;
}