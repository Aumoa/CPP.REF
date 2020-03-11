#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 단일 키프레임을 표현합니다. </summary>
	template< class T = Vector3 >
	struct Keyframe
	{
		/// <summary> 키프레임의 시간을 초 단위로 설정합니다. </summary>
		double TimePos;

		/// <summary> 키프레임의 변형 값을 설정합니다. </summary>
		T Value;

		/// <summary> <see cref="Keyframe"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Keyframe() = default;

		/// <summary> 시간 경과값이 현재 키프레임과 대상 키프레임 사이에 위치하는지 검사합니다. </summary>
		bool CheckIf( const Keyframe& keyf, double timePos ) const
		{
			return timePos >= TimePos && timePos < keyf.TimePos;
		}

		/// <summary> 시간 경과를 사용하여 대상 키프레임과 보간합니다. </summary>
		/// <param name="keyf"> 대상 키프레임을 전달합니다. </param>
		/// <param name="timePos"> 시간 경과값을 전달합니다. </param>
		Keyframe Interpolate( const Keyframe& keyf, double timePos ) const
		{
			auto deltaT = keyf.TimePos - TimePos;
			auto deltaV = keyf.Value - Value;

			auto t = ( timePos - TimePos ) / deltaT;
			
			Keyframe k;
			k.TimePos = timePos;

			if constexpr ( std::is_same_v<T, Quaternion> )
			{
				k.Value = Value.Slerp( keyf.Value, t );
			}
			else
			{
				k.Value = Value.Lerp( keyf.Value, t );
			}

			return std::move( k );
		}

		Keyframe& operator=( const Keyframe& copy )
		{
			TimePos = copy.TimePos;
			Value = copy.Value;
			return *this;
		}

		constexpr bool operator==( const Keyframe& right ) const
		{
			return TimePos == right.TimePos && Value == right.Value;
		}

		constexpr bool operator!=( const Keyframe& right ) const
		{
			return TimePos != right.TimePos || Value != right.Value;
		}
	};
}

template< class T >
inline std::wostream& operator<<( std::wostream& wout, const SC::Game::Keyframe<T>& keyframe )
{
	wout << L"TimePos: " << keyframe.TimePos;
	return wout << L", Value: " << keyframe.Value;
}