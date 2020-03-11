#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 장면을 표현합니다. </summary>
	class AnimationClip : public Assets
	{
		friend class Animator;

		std::map<std::string, Keyframes, std::less<>> keyframes;
		double duration = 0;
		bool isLoop = false;

		std::map<std::string, Keyframes::KeyframePair, std::less<>> MakeInterpolateMap();
		void Interpolate( double timePos, std::map<std::string, Keyframes::KeyframePair, std::less<>>& interpolated );

	public:
		/// <summary> <see cref="AnimationClip"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 전달합니다. </param>
		AnimationClip( String name );

		/// <summary> 뼈대 키프레임 목록에서 뼈대에 관련된 키프레임 목록을 제거합니다. </summary>
		/// <param name="boneName"> 뼈대 이름을 전달합니다. </param>
		void RemoveKeyframes( const std::string_view& boneName );

		/// <summary> (Visual Studio 전용) 뼈대의 애니메이션 키프레임 목록을 설정하거나 가져옵니다. </summary>
		vs_property( Game::Keyframes, Keyframes )[];

		/// <summary> 뼈대의 애니메이션 키프레임 목록을 가져옵니다. </summary>
		Game::Keyframes Keyframes_get( const std::string_view& param0 );

		/// <summary> 뼈대의 애니메이션 키프레임 목록을 설정합니다. </summary>
		void Keyframes_set( const std::string_view& param0, const Game::Keyframes& value );

		/// <summary> 뼈대의 애니메이션 키프레임 목록을 설정합니다. </summary>
		void Keyframes_set( const std::string_view& param0, Game::Keyframes&& value );

		/// <summary> (Visual Studio 전용) 애니메이션의 마지막 키프레임의 시간 위치를 가져옵니다. </summary>
		vs_property( double, Duration );

		/// <summary> 애니메이션의 마지막 키프레임의 시간 위치를 가져옵니다. </summary>
		double Duration_get();

		/// <summary> (Visual Studio 전용) 애니메이션이 반복하도록 설정하는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsLoop );

		/// <summary> 애니메이션이 반복하도록 설정하는 값을 가져옵니다. </summary>
		bool IsLoop_get();

		/// <summary> 애니메이션이 반복하도록 설정하는 값을 설정합니다. </summary>
		void IsLoop_set( bool value );

	private:
		void ComputeDur();
	};
}