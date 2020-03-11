#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� ���� ��ȯ�� ���� �Ű����� �� ������ ǥ���մϴ�. </summary>
	struct AnimationTransitionCondition
	{
		/// <summary> ��ȯ�� �ִϸ��̼� ���� �̸��� �����մϴ�. </summary>
		std::wstring To;

		/// <summary> ���� �Ű����� �̸��� �����մϴ�. </summary>
		std::wstring VarName;

		/// <summary> ������ �׽�Ʈ�� �Լ��� �����մϴ�. </summary>
		std::function<bool( AnimVar var )> Condition;

		/// <summary> �� ��ȯ ���´� ���� ��� ���� �ִϸ��̼� ����� �Ϸ�� ������ ����ؾ� �մϴ�. </summary>
		bool HasExitTime = false;

		/// <summary> �� ��ȯ�� ����Ǵ� �ð��� �����մϴ�. </summary>
		double BlendTime = 0;


		/// <summary> <see cref="AnimationTransitionCondition"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		AnimationTransitionCondition()
		{

		}

		/// <summary> <see cref="AnimationTransitionCondition"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		AnimationTransitionCondition( const std::wstring_view& to, const std::wstring_view& varName, double blendTime, std::function<bool( AnimVar )> condition )
			: To( to )
			, VarName( varName )
			, BlendTime( blendTime )
			, Condition( condition )
		{

		}
	};
}