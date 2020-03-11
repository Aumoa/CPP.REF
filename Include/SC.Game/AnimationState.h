#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� ���¸� ��Ÿ���ϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class AnimationState final : public Assets
	{
		RefPtr<AnimationClip> clip;

	public:
		/// <summary> <see cref="AnimationState"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ������ �ڻ� �̸��� �����մϴ�. </param>
		AnimationState( String name );

		/// <summary> (Visual Studio ����) ��� ��� �ִϸ��̼� Ŭ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<AnimationClip>, Clip );

		/// <summary> ��� ��� �ִϸ��̼� Ŭ���� �����ɴϴ�. </summary>
		RefPtr<AnimationClip> Clip_get();

		/// <summary> ��� ��� �ִϸ��̼� Ŭ���� �����մϴ�. </summary>
		void Clip_set( RefPtr<AnimationClip> value );
	};
}