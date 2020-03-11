#pragma once

namespace SC::Game
{
	/// <summary> 계층 구조 뼈대를 표현합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Bone final : public Component
	{
		std::string name;
		int index = 0;
		bool restore = false;

	public:
		/// <summary> <see cref="Bone"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Bone();

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> 클래스에서 상속 됨.) 컴포넌트에 대한 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		void Update( Time& time, Input& input ) override;

		/// <summary> (Visual Studio 전용) 뼈대의 이름을 설정하거나 가져옵니다. </summary>
		vs_property( std::string, Name );

		/// <summary> 뼈대의 이름을 가져옵니다. </summary>
		std::string Name_get();

		/// <summary> 뼈대의 이름을 설정합니다. </summary>
		void Name_set( const std::string_view& value0 );

		/// <summary> (Visual Studio 전용) 뼈대의 색인 번호를 설정하거나 가져옵니다. </summary>
		vs_property( int, Index );

		/// <summary> 뼈대의 색인 번호를 가져옵니다. </summary>
		int Index_get();

		/// <summary> 뼈대의 색인 번호를 설정합니다. </summary>
		void Index_set( int value );

		/// <summary> (Visual Studio 전용) 뼈대가 복원되어야 함을 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, Restore );

		/// <summary> 뼈대가 복원되어야 함을 나타내는 값을 가져옵니다. </summary>
		bool Restore_get();

		/// <summary> 뼈대가 복원되어야 함을 나타내는 값을 설정합니다. </summary>
		void Restore_set( bool value );
	};
}