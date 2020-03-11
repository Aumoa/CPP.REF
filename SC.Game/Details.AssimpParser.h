#pragma once

namespace SC::Game::Details
{
	class AssimpParser : virtual public Object
	{
		Assimp::Importer importer;

		const aiScene* scene = nullptr;
		std::filesystem::path directory;

		struct tag_Joint
		{
			std::string Name;
			XMFLOAT4X4 Offset;
			int Parent = -1;
			bool Restore = false;
		};

		struct tag_Mesh
		{
			RefPtr<Mesh> pMesh;
			int JointStart = 0;
			int JointCount = 0;
		};

		std::vector<RefPtr<Material>> materials;
		std::vector<tag_Joint> joints;
		std::vector<tag_Mesh> meshes;
		RefPtr<AnimatorController> controller;

	public:
		AssimpParser( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg );

		RefPtr<GameObject> TryParse( const std::wstring_view& name );

	private:
		void ProcessMaterials( const std::wstring_view& name, const aiScene* scene );
		void ProcessMeshes( RefPtr<GameObject> gameObject, const aiScene* scene );
		void ProcessJointHierarchy( RefPtr<GameObject> gameObject, aiNode* node, int jointParent = -1 );
		void ProcessAnimations( const std::wstring_view& name, const aiScene* scene );

		void AddJointIndex( SkinnedVertex& v, int jointIndex, float weight );
		int SearchJoint( const std::string_view& name );
	};
}