#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� ���� Ű�������� ǥ���մϴ�. </summary>
	template< class T = Vector3 >
	struct Keyframe
	{
		/// <summary> Ű�������� �ð��� �� ������ �����մϴ�. </summary>
		double TimePos;

		/// <summary> Ű�������� ���� ���� �����մϴ�. </summary>
		T Value;

		/// <summary> <see cref="Keyframe"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Keyframe() = default;

		/// <summary> �ð� ������� ���� Ű�����Ӱ� ��� Ű������ ���̿� ��ġ�ϴ��� �˻��մϴ�. </summary>
		bool CheckIf( const Keyframe& keyf, double timePos ) const
		{
			return timePos >= TimePos && timePos < keyf.TimePos;
		}

		/// <summary> �ð� ����� ����Ͽ� ��� Ű�����Ӱ� �����մϴ�. </summary>
		/// <param name="keyf"> ��� Ű�������� �����մϴ�. </param>
		/// <param name="timePos"> �ð� ������� �����մϴ�. </param>
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