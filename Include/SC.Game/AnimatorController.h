#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 상태를 제어하는 애니메이션 상태 집합을 표현합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class AnimatorController final : public Assets
	{
		friend class Animator;

		std::map<std::wstring, RefPtr<AnimationState>, std::less<>> animationStates;
		std::map<std::wstring, AnimVar, std::less<>> parameters;
		std::map<std::wstring, std::vector<AnimationTransitionCondition>, std::less<>> transConds;

		std::wstring_view defaultState;

		decltype( parameters ) MakeParameters();

	public:
		/// <summary> <see cref="AnimatorController"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 전달합니다. </param>
		AnimatorController( String name );

		/// <summary> 애니메이션 상태 개체를 추가합니다. </summary>
		/// <param name="state"> 상태 개체를 전달합니다. 개체의 Assets::Name 값이 이름으로 사용됩니다. </param>
		/// <param name="isDefault"> 개체를 기본 상태로 사용합니다. </param>
		void AddState( RefPtr<AnimationState> state, bool isDefault = false );

		/// <summary> 애니메이션 상태 개체를 추가합니다. </summary>
		/// <param name="stateNameOverride"> 덮어쓸 새로운 상태 개체 이름을 전달합니다. </param>
		/// <param name="state"> 상태 개체를 전달합니다. </param>
		/// <param name="isDefault"> 개체를 기본 상태로 사용합니다. </param>
		void AddState( const std::wstring_view& stateNameOverride, RefPtr<AnimationState> state, bool isDefault = false );

		/// <summary> 애니메이션 상태 개체를 제거합니다. </summary>
		/// <param name="stateName"> 상태 개체 이름을 전달합니다. </param>
		void RemoveState( const std::wstring_view& stateName );

		/// <summary> 새 애니메이션 매개변수를 추가합니다. </summary>
		/// <param name="var"> 초기화 값이 포함된 매개변수 값을 전달합니다. </param>
		void AddVar( const std::wstring_view& name, const AnimVar& var );

		/// <summary> 애니메이션 매개변수를 제거합니다. </summary>
		/// <param name="name"> 매개변수 이름을 전달합니다. </param>
		void RemoveVar( const std::wstring_view& name );

		/// <summary> 애니메이션 전환 상태를 추가합니다. </summary>
		/// <param name="from"> 시작 애니메이션 상태 개체의 등록 이름을 전달합니다. </param>
		/// <param name="value"> 애니메이션 전환 조건 구조체를 전달합니다. </param>
		void AddTransition( const std::wstring_view& from, const AnimationTransitionCondition& value );

		/// <summary> 애니메이션 전환 상태를 제거합니다. </summary>
		/// <param name="from"> 시작 애니메이션 상태 개체의 등록 이름을 전달합니다. </param>
		/// <param name="value"> 애니메이션 전환 조건 구조체를 전달합니다. 값이 비어있을 경우 최상위 탐색 요소를 사용합니다. </param>
		void RemoveTransition( const std::wstring_view& from, const AnimationTransitionCondition& value = AnimationTransitionCondition() );

		/// <summary> (Visual Studio 전용) 애니메이션 상태 개체를 가져옵니다. </summary>
		vs_property_get( RefPtr<AnimationState>, States )[];

		/// <summary> 애니메이션 상태 개체를 가져옵니다. </summary>
		RefPtr<AnimationState> States_get( const std::wstring_view& param0 );

		/// <summary> (Visual Studio 전용) 애니메이션 매개변수 값을 가져옵니다. </summary>
		vs_property_get( AnimVar, Vars )[];

		/// <summary> 애니메이션 매개변수 값을 가져옵니다. </summary>
		AnimVar Vars_get( const std::wstring_view& param0 );

		/// <summary> (Visual Studio 전용) 기본 애니메이션 상태 개체 이름을 가져옵니다. </summary>
		vs_property_get( std::wstring_view, DefaultState );

		/// <summary> 기본 애니메이션 상태 개체 이름을 가져옵니다. </summary>
		std::wstring_view DefaultState_get();

		/// <summary> (Visual Studio 전용) 애니메이션 전환 상태를 가져옵니다. </summary>
		vs_property_get( std::vector<AnimationTransitionCondition>, Transition )[];

		///  <summary> 애니메이션 전환 상태를 가져옵니다. </summary>
		std::vector<AnimationTransitionCondition> Transition_get( const std::wstring_view& param0 );
	};
}