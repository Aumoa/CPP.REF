#pragma once

namespace SC::Game
{
	/// <summary> MeshFilter ������Ʈ���� ������ Mesh ��ü�� ȭ�鿡 �������մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class MeshRenderer : public Component
	{
		static RefPtr<Material> defaultMaterial;
		static uint64 reference_count;
		RefPtr<Material> material;

	protected:
		sc_game_export_symbol void Render( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="MeshRenderer"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MeshRenderer();
		~MeshRenderer();

		/// <summary> (<see cref="ICloneable"/>) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (Visual Studio ����) ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Game::Material>, Material );

		/// <summary> ������ �����ɴϴ�. </summary>
		RefPtr<Game::Material> Material_get();

		/// <summary> ������ �����մϴ�. </summary>
		void Material_set( RefPtr<Game::Material> value );
	};
}