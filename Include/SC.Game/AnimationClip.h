#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� ����� ǥ���մϴ�. </summary>
	class AnimationClip : public Assets
	{
		friend class Animator;

		std::map<std::string, Keyframes, std::less<>> keyframes;
		double duration = 0;
		bool isLoop = false;

		std::map<std::string, Keyframes::KeyframePair, std::less<>> MakeInterpolateMap();
		void Interpolate( double timePos, std::map<std::string, Keyframes::KeyframePair, std::less<>>& interpolated );

	public:
		/// <summary> <see cref="AnimationClip"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ������ �ڻ� �̸��� �����մϴ�. </param>
		AnimationClip( String name );

		/// <summary> ���� Ű������ ��Ͽ��� ���뿡 ���õ� Ű������ ����� �����մϴ�. </summary>
		/// <param name="boneName"> ���� �̸��� �����մϴ�. </param>
		void RemoveKeyframes( const std::string_view& boneName );

		/// <summary> (Visual Studio ����) ������ �ִϸ��̼� Ű������ ����� �����ϰų� �����ɴϴ�. </summary>
		vs_property( Game::Keyframes, Keyframes )[];

		/// <summary> ������ �ִϸ��̼� Ű������ ����� �����ɴϴ�. </summary>
		Game::Keyframes Keyframes_get( const std::string_view& param0 );

		/// <summary> ������ �ִϸ��̼� Ű������ ����� �����մϴ�. </summary>
		void Keyframes_set( const std::string_view& param0, const Game::Keyframes& value );

		/// <summary> ������ �ִϸ��̼� Ű������ ����� �����մϴ�. </summary>
		void Keyframes_set( const std::string_view& param0, Game::Keyframes&& value );

		/// <summary> (Visual Studio ����) �ִϸ��̼��� ������ Ű�������� �ð� ��ġ�� �����ɴϴ�. </summary>
		vs_property( double, Duration );

		/// <summary> �ִϸ��̼��� ������ Ű�������� �ð� ��ġ�� �����ɴϴ�. </summary>
		double Duration_get();

		/// <summary> (Visual Studio ����) �ִϸ��̼��� �ݺ��ϵ��� �����ϴ� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsLoop );

		/// <summary> �ִϸ��̼��� �ݺ��ϵ��� �����ϴ� ���� �����ɴϴ�. </summary>
		bool IsLoop_get();

		/// <summary> �ִϸ��̼��� �ݺ��ϵ��� �����ϴ� ���� �����մϴ�. </summary>
		void IsLoop_set( bool value );

	private:
		void ComputeDur();
	};
}