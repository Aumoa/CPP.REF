#pragma once

namespace SC::Game
{
	/// <summary> Mesh 데이터를 사용하는 컴포넌트에서 Mesh 데이터를 참조하기 위해 사용하는 클래스입니다. 이 클래스는 상속될 수 없습니다. </summary>
	class MeshFilter : public Component
	{
		RefPtr<Mesh> mesh;

	public:
		/// <summary> <see cref="MeshFilter"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		MeshFilter();

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (Visual Studio 전용) 참조 Mesh 개체를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Game::Mesh>, Mesh );

		/// <summary> 참조 Mesh 개체를 가져옵니다. </summary>
		RefPtr<Game::Mesh> Mesh_get();

		/// <summary> 참조 Mesh 개체를 설정합니다. </summary>
		void Mesh_set( RefPtr<Game::Mesh> value );
	};
}