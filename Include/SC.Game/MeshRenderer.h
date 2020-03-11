#pragma once

namespace SC::Game
{
	/// <summary> MeshFilter 컴포넌트에서 지정된 Mesh 개체를 화면에 렌더링합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class MeshRenderer : public Component
	{
		static RefPtr<Material> defaultMaterial;
		static uint64 reference_count;
		RefPtr<Material> material;

	protected:
		sc_game_export_symbol void Render( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="MeshRenderer"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		MeshRenderer();
		~MeshRenderer();

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (Visual Studio 전용) 재질을 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Game::Material>, Material );

		/// <summary> 재질을 가져옵니다. </summary>
		RefPtr<Game::Material> Material_get();

		/// <summary> 재질을 설정합니다. </summary>
		void Material_set( RefPtr<Game::Material> value );
	};
}