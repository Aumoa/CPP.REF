using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI::Shapes;
using namespace SC::Game::Details;

Rect<double> Rectangle::OnUpdate( Rect<double> clientRect )
{
	return clientRect;
}

void Rectangle::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	if ( Fill.IsValid )
	{
		auto pCommandList = deviceContext->pCommandList.Get();

		if ( auto slot = deviceContext->Slot["Brush"]; slot != -1 )
		{
			pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, Fill->brushConstants->VirtualAddress );
		}
		
		if ( auto slot = deviceContext->Slot["RenderType"]; slot != -1 )
		{
			pCommandList->SetGraphicsRoot32BitConstant( ( UINT )slot, 1, 0 );
		}

		auto rc = ( Rect<float> )ActualContentRect;

		tag_ShaderInfo shaderInfo;
		shaderInfo.output.left = rc.Left;
		shaderInfo.output.top = rc.Top;
		shaderInfo.output.width = rc.Width;
		shaderInfo.output.height = rc.Height;
		deviceContext->DispatchShaderInfo( 1, &shaderInfo );
	}
}

Rectangle::Rectangle( String name )
	: Shape( name )
{
	Width = 100;
	Height = 100;
}

double Rectangle::RadiusX_get()
{
	return radiusX;
}

void Rectangle::RadiusX_set( double value )
{
	radiusX = value;
}

double Rectangle::RadiusY_get()
{
	return radiusY;
}

void Rectangle::RadiusY_set( double value )
{
	radiusY = value;
}