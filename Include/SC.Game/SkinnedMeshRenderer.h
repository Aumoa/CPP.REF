#pragma once

namespace SC::Game
{
	/// <summary> ������ Mesh ��ü�� ȭ�鿡 �������մϴ�. ��ü�� IsSkinned �Ӽ��� true ���� �����ϴ�. </summary>
	class SkinnedMeshRenderer : public Component
	{
		static RefPtr<Material> defaultMaterial;
		static uint64 reference_count;
		RefPtr<Mesh> mesh;
		RefPtr<Material> material;

	protected:
		void Render( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="SkinnedMeshRenderer"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		SkinnedMeshRenderer();
		~SkinnedMeshRenderer() override;

		/// <summary> (<see cref="ICloneable"/>) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (Visual Studio ����) ���� Mesh ��ü�� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Game::Mesh>, Mesh );

		/// <summary> ���� Mesh ��ü�� �����ɴϴ�. </summary>
		RefPtr<Game::Mesh> Mesh_get();

		/// <summary> ���� Mesh ��ü�� �����մϴ�. </summary>
		void Mesh_set( RefPtr<Game::Mesh> value );

		/// <summary> (Visual Studio ����) ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Game::Material>, Material );

		/// <summary> ������ �����ɴϴ�. </summary>
		RefPtr<Game::Material> Material_get();

		/// <summary> ������ �����մϴ�. </summary>
		void Material_set( RefPtr<Game::Material> value );
	};
}