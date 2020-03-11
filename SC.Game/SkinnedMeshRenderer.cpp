using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

RefPtr<Material> SkinnedMeshRenderer::defaultMaterial;
uint64 SkinnedMeshRenderer::reference_count = 0;

void SkinnedMeshRenderer::Render( RefPtr<CDeviceContext>& deviceContext )
{
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

SkinnedMeshRenderer::SkinnedMeshRenderer()
{
	if ( !defaultMaterial.IsValid )
	{
		defaultMaterial = new Game::Material( "defaultMaterial" );
	}
	reference_count += 1;
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	if ( ( reference_count -= 1 ) == 0 )
	{
		defaultMaterial = nullptr;
	}
}

object SkinnedMeshRenderer::Clone()
{
	RefPtr renderer = new SkinnedMeshRenderer();
	renderer->material = material;
	renderer->mesh = mesh;
	return renderer;
}

RefPtr<Mesh> SkinnedMeshRenderer::Mesh_get()
{
	return mesh;
}

void SkinnedMeshRenderer::Mesh_set( RefPtr<Game::Mesh> value )
{
	mesh = value;
}

RefPtr<Material> SkinnedMeshRenderer::Material_get()
{
	return material;
}

void SkinnedMeshRenderer::Material_set( RefPtr<Game::Material> value )
{
	material = value;
}