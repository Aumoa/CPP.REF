#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 상태 전환을 위한 매개변수 및 조건을 표현합니다. </summary>
	struct AnimationTransitionCondition
	{
		/// <summary> 전환할 애니메이션 상태 이름을 설정합니다. </summary>
		std::wstring To;

		/// <summary> 조건 매개변수 이름을 설정합니다. </summary>
		std::wstring VarName;

		/// <summary> 조건을 테스트할 함수를 설정합니다. </summary>
		std::function<bool( AnimVar var )> Condition;

		/// <summary> 이 전환 상태는 현재 재생 중인 애니메이션 재생이 완료될 때까지 대기해야 합니다. </summary>
		bool HasExitTime = false;

		/// <summary> 이 전환이 진행되는 시간을 설정합니다. </summary>
		double BlendTime = 0;


		/// <summary> <see cref="AnimationTransitionCondition"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		AnimationTransitionCondition()
		{

		}

		/// <summary> <see cref="AnimationTransitionCondition"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		AnimationTransitionCondition( const std::wstring_view& to, const std::wstring_view& varName, double blendTime, std::function<bool( AnimVar )> condition )
			: To( to )
			, VarName( varName )
			, BlendTime( blendTime )
			, Condition( condition )
		{

		}
	};
}