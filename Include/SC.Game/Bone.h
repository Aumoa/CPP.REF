#pragma once

namespace SC::Game
{
	/// <summary> ���� ���� ���븦 ǥ���մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class Bone final : public Component
	{
		std::string name;
		int index = 0;
		bool restore = false;

	public:
		/// <summary> <see cref="Bone"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Bone();

		/// <summary> (<see cref="ICloneable"/>) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> Ŭ�������� ��� ��.) ������Ʈ�� ���� �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		/// <param name="input"> ���� �����ӿ��� �Է� ���¸� �����մϴ�. </param>
		void Update( Time& time, Input& input ) override;

		/// <summary> (Visual Studio ����) ������ �̸��� �����ϰų� �����ɴϴ�. </summary>
		vs_property( std::string, Name );

		/// <summary> ������ �̸��� �����ɴϴ�. </summary>
		std::string Name_get();

		/// <summary> ������ �̸��� �����մϴ�. </summary>
		void Name_set( const std::string_view& value0 );

		/// <summary> (Visual Studio ����) ������ ���� ��ȣ�� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, Index );

		/// <summary> ������ ���� ��ȣ�� �����ɴϴ�. </summary>
		int Index_get();

		/// <summary> ������ ���� ��ȣ�� �����մϴ�. </summary>
		void Index_set( int value );

		/// <summary> (Visual Studio ����) ���밡 �����Ǿ�� ���� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, Restore );

		/// <summary> ���밡 �����Ǿ�� ���� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool Restore_get();

		/// <summary> ���밡 �����Ǿ�� ���� ��Ÿ���� ���� �����մϴ�. </summary>
		void Restore_set( bool value );
	};
}