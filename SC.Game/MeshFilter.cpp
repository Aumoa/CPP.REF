using namespace SC;
using namespace SC::Game;

MeshFilter::MeshFilter()
{

}

object MeshFilter::Clone()
{
	RefPtr meshFilter = new MeshFilter();
	meshFilter->mesh = mesh;
	return meshFilter;
}

RefPtr<Mesh> MeshFilter::Mesh_get()
{
	return mesh;
}

void MeshFilter::Mesh_set( RefPtr<Game::Mesh> value )
{
	mesh = value;
}