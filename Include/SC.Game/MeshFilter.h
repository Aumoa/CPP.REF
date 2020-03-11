#pragma once

namespace SC::Game
{
	/// <summary> Mesh �����͸� ����ϴ� ������Ʈ���� Mesh �����͸� �����ϱ� ���� ����ϴ� Ŭ�����Դϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class MeshFilter : public Component
	{
		RefPtr<Mesh> mesh;

	public:
		/// <summary> <see cref="MeshFilter"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MeshFilter();

		/// <summary> (<see cref="ICloneable"/>) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (Visual Studio ����) ���� Mesh ��ü�� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Game::Mesh>, Mesh );

		/// <summary> ���� Mesh ��ü�� �����ɴϴ�. </summary>
		RefPtr<Game::Mesh> Mesh_get();

		/// <summary> ���� Mesh ��ü�� �����մϴ�. </summary>
		void Mesh_set( RefPtr<Game::Mesh> value );
	};
}