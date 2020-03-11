#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 상태를 나타냅니다. 이 클래스는 상속될 수 없습니다. </summary>
	class AnimationState final : public Assets
	{
		RefPtr<AnimationClip> clip;

	public:
		/// <summary> <see cref="AnimationState"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 전달합니다. </param>
		AnimationState( String name );

		/// <summary> (Visual Studio 전용) 재생 대상 애니메이션 클립을 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<AnimationClip>, Clip );

		/// <summary> 재생 대상 애니메이션 클립을 가져옵니다. </summary>
		RefPtr<AnimationClip> Clip_get();

		/// <summary> 재생 대상 애니메이션 클립을 설정합니다. </summary>
		void Clip_set( RefPtr<AnimationClip> value );
	};
}