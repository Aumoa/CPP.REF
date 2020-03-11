using namespace SC;
using namespace SC::Game;
using namespace SC::Game::UI;
using namespace SC::Game::Details;
using namespace SC::Drawing;

using namespace std;

Rect<double> Image::OnUpdate( Rect<double> clientRect )
{
	rectangle->Update( clientRect );
	return clientRect;
}

void Image::OnRender( RefPtr<CDeviceContext>& deviceContext )
{
	if ( imageSource->IsValid )
	{
		auto pCommandList = deviceContext->pCommandList.Get();

		if ( auto slot = deviceContext->Slot["Texture"]; slot != -1 )
		{
			deviceContext->SetGraphicsRootShaderResources( slot, imageSource->pShaderResourceView );

			if ( slot = deviceContext->Slot["RenderType"]; slot != -1 )
			{
				struct tag
				{
					int Type;
					float Opacity;
				} tag_RenderType;

				tag_RenderType.Type = 2;
				tag_RenderType.Opacity = ( float )opacity;
				pCommandList->SetGraphicsRoot32BitConstants( ( UINT )slot, 2, &tag_RenderType, 0 );

				auto client = ( Rect<float> )ActualContentRect;

				tag_ShaderInfo shaderInfo;
				shaderInfo.input.left = 0;
				shaderInfo.input.top = 0;
				shaderInfo.input.width = 1.0f;
				shaderInfo.input.height = 1.0f;
				shaderInfo.output.left = client.Left;
				shaderInfo.output.top = client.Top;
				shaderInfo.output.width = client.Width;
				shaderInfo.output.height = client.Height;

				deviceContext->DispatchShaderInfo( 1, &shaderInfo );
			}
		}
	}
	else
	{
		rectangle->Render( deviceContext );
	}
}

Image::Image( String name ) : Element( name )
{
	rectangle = new Shapes::Rectangle( "empty_image_fill" );
	rectangle->Fill = new SolidColorBrush( Color::White );
}

RefPtr<Texture2D> Image::Source_get()
{
	return imageSource;
}

void Image::Source_set( RefPtr<Texture2D> value )
{
	imageSource = value;
}

double Image::Opacity_get()
{
	return opacity;
}

void Image::Opacity_set( double value )
{
	opacity = std::clamp( value, 0.0, 1.0 );
}