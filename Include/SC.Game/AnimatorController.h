#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� ���¸� �����ϴ� �ִϸ��̼� ���� ������ ǥ���մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class AnimatorController final : public Assets
	{
		friend class Animator;

		std::map<std::wstring, RefPtr<AnimationState>, std::less<>> animationStates;
		std::map<std::wstring, AnimVar, std::less<>> parameters;
		std::map<std::wstring, std::vector<AnimationTransitionCondition>, std::less<>> transConds;

		std::wstring_view defaultState;

		decltype( parameters ) MakeParameters();

	public:
		/// <summary> <see cref="AnimatorController"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ������ �ڻ� �̸��� �����մϴ�. </param>
		AnimatorController( String name );

		/// <summary> �ִϸ��̼� ���� ��ü�� �߰��մϴ�. </summary>
		/// <param name="state"> ���� ��ü�� �����մϴ�. ��ü�� Assets::Name ���� �̸����� ���˴ϴ�. </param>
		/// <param name="isDefault"> ��ü�� �⺻ ���·� ����մϴ�. </param>
		void AddState( RefPtr<AnimationState> state, bool isDefault = false );

		/// <summary> �ִϸ��̼� ���� ��ü�� �߰��մϴ�. </summary>
		/// <param name="stateNameOverride"> ��� ���ο� ���� ��ü �̸��� �����մϴ�. </param>
		/// <param name="state"> ���� ��ü�� �����մϴ�. </param>
		/// <param name="isDefault"> ��ü�� �⺻ ���·� ����մϴ�. </param>
		void AddState( const std::wstring_view& stateNameOverride, RefPtr<AnimationState> state, bool isDefault = false );

		/// <summary> �ִϸ��̼� ���� ��ü�� �����մϴ�. </summary>
		/// <param name="stateName"> ���� ��ü �̸��� �����մϴ�. </param>
		void RemoveState( const std::wstring_view& stateName );

		/// <summary> �� �ִϸ��̼� �Ű������� �߰��մϴ�. </summary>
		/// <param name="var"> �ʱ�ȭ ���� ���Ե� �Ű����� ���� �����մϴ�. </param>
		void AddVar( const std::wstring_view& name, const AnimVar& var );

		/// <summary> �ִϸ��̼� �Ű������� �����մϴ�. </summary>
		/// <param name="name"> �Ű����� �̸��� �����մϴ�. </param>
		void RemoveVar( const std::wstring_view& name );

		/// <summary> �ִϸ��̼� ��ȯ ���¸� �߰��մϴ�. </summary>
		/// <param name="from"> ���� �ִϸ��̼� ���� ��ü�� ��� �̸��� �����մϴ�. </param>
		/// <param name="value"> �ִϸ��̼� ��ȯ ���� ����ü�� �����մϴ�. </param>
		void AddTransition( const std::wstring_view& from, const AnimationTransitionCondition& value );

		/// <summary> �ִϸ��̼� ��ȯ ���¸� �����մϴ�. </summary>
		/// <param name="from"> ���� �ִϸ��̼� ���� ��ü�� ��� �̸��� �����մϴ�. </param>
		/// <param name="value"> �ִϸ��̼� ��ȯ ���� ����ü�� �����մϴ�. ���� ������� ��� �ֻ��� Ž�� ��Ҹ� ����մϴ�. </param>
		void RemoveTransition( const std::wstring_view& from, const AnimationTransitionCondition& value = AnimationTransitionCondition() );

		/// <summary> (Visual Studio ����) �ִϸ��̼� ���� ��ü�� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<AnimationState>, States )[];

		/// <summary> �ִϸ��̼� ���� ��ü�� �����ɴϴ�. </summary>
		RefPtr<AnimationState> States_get( const std::wstring_view& param0 );

		/// <summary> (Visual Studio ����) �ִϸ��̼� �Ű����� ���� �����ɴϴ�. </summary>
		vs_property_get( AnimVar, Vars )[];

		/// <summary> �ִϸ��̼� �Ű����� ���� �����ɴϴ�. </summary>
		AnimVar Vars_get( const std::wstring_view& param0 );

		/// <summary> (Visual Studio ����) �⺻ �ִϸ��̼� ���� ��ü �̸��� �����ɴϴ�. </summary>
		vs_property_get( std::wstring_view, DefaultState );

		/// <summary> �⺻ �ִϸ��̼� ���� ��ü �̸��� �����ɴϴ�. </summary>
		std::wstring_view DefaultState_get();

		/// <summary> (Visual Studio ����) �ִϸ��̼� ��ȯ ���¸� �����ɴϴ�. </summary>
		vs_property_get( std::vector<AnimationTransitionCondition>, Transition )[];

		///  <summary> �ִϸ��̼� ��ȯ ���¸� �����ɴϴ�. </summary>
		std::vector<AnimationTransitionCondition> Transition_get( const std::wstring_view& param0 );
	};
}