using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

RefPtr<Material> MeshRenderer::defaultMaterial;
uint64 MeshRenderer::reference_count = 0;

void MeshRenderer::Render( RefPtr<CDeviceContext>& deviceContext )
{
	auto meshFilter = GetComponent<MeshFilter>();

	if ( meshFilter.IsValid )
	{
		auto mesh = meshFilter->Mesh;
		if ( mesh.IsValid )
		{
			if ( material.IsValid )
			{
				material->SetGraphicsRootConstantBuffers( deviceContext );
			}
			else
			{
				defaultMaterial->SetGraphicsRootConstantBuffers( deviceContext );
			}

			mesh->DrawIndexed( deviceContext );
		}
	}
}

MeshRenderer::MeshRenderer()
{
	if ( !defaultMaterial.IsValid )
	{
		defaultMaterial = new Game::Material( "defaultMaterial" );
	}
	reference_count += 1;
}

MeshRenderer::~MeshRenderer()
{
	if ( ( reference_count -= 1 ) == 0 )
	{
		defaultMaterial = nullptr;
	}
}

object MeshRenderer::Clone()
{
	RefPtr renderer = new MeshRenderer();
	renderer->material = material;
	return renderer;
}

RefPtr<Material> MeshRenderer::Material_get()
{
	return material;
}

void MeshRenderer::Material_set( RefPtr<Game::Material> value )
{
	material = value;
}