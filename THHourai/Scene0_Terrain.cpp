using namespace Touhou;

using namespace SC::Drawing;

Scene0_Terrain::Scene0_Terrain( String name ) : GameObject( name )
{
	IsLocked = true;
	InitializeComponents();
	IsLocked = false;
}

void Scene0_Terrain::InitializeComponents()
{
	// ¹Ù´Ú
	declare_element( GameObject, terrain );
	terrain->AddComponent<MeshFilter>()->Mesh = Mesh::CreatePlane( "Scene01_Terrain_Plane", 100.0f, 100.0f );
	{
		declare_element( Material, terrain_material );
		terrain_material->DiffuseMap = new Texture2D( "terrain_material_diffuse", "Assets/Texture/templeBase01_diffuse.jpg" );
		terrain_material->NormalMap = new Texture2D( "terrain_material_diffuse", "Assets/Texture/templeBase01_normal.jpg" );

		terrain->AddComponent<MeshRenderer>()->Material = terrain_material;
	}
	{
		auto trp = terrain->Transform;
		trp->Scale = Vector3( 100, 1, 100 );
		trp->Position = Vector3( 94, 0, -94 );
	}
	terrain->Parent = this;

	// µ¹
	var rock1 = AssetBundle::LoadModelAssetsAsync( "Assets/Model/Rock/scene.gltf" );
	var rock1_obj = rock1->GetResults().As<GameObject>();
	rock1_obj->Transform->Scale = 0.3;
	rock1_obj->Transform->Position = Vector3( -5, 1, 3 );
	rock1_obj->Transform->Rotation = Quaternion::AngleAxis( 3.14 * 0.5, Vector3::Right );
	rock1_obj->Parent = this;

	rock1_obj = rock1_obj->Clone().As<GameObject>();
	rock1_obj->Transform->Position = Vector3( -6, 1, -1 );
	rock1_obj->Parent = this;

	rock1_obj = rock1_obj->Clone().As<GameObject>();
	rock1_obj->Transform->Position = Vector3( -5, 1, -5 );
	rock1_obj->Parent = this;
}