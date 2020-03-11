#pragma once

namespace SC::Game
{
	/// <summary> 게임 엔진에서 시간 참조를 위해 사용하는 값 집합입니다. </summary>
	struct Time : public ValueType
	{
		friend class Scene;

	private:
		double time;
		double fixedTime;
		double deltaTime;
		double fixedDeltaTime;

	public:
		/// <summary> <see cref="Time"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Time()
			: time( 0 )
			, fixedTime( 0 )
			, deltaTime( 0 )
			, fixedDeltaTime( 0 )
		{

		}

		/// <summary> <see cref="Time"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Time( const Time& copy )
			: time( copy.time )
			, fixedTime( copy.fixedTime )
			, deltaTime( copy.deltaTime )
			, fixedDeltaTime( copy.fixedDeltaTime )
		{

		}

		/// <summary> (Visual Studio 전용) 장면이 시작된 이후 경과된 시간을 초 단위로 반환합니다. </summary>
		vs_property_get( double, TimeInSeconds );

		/// <summary> 장면이 시작된 이후 경과된 시간을 초 단위로 반환합니다. </summary>
		double TimeInSeconds_get()
		{
			return time;
		}

		/// <summary> (Visual Studio 전용) 장면이 시작된 이후 경과된 고정 프레임 시간을 초 단위로 반환합니다. </summary>
		vs_property_get( double, FixedTimeInSeconds );

		/// <summary> 장면이 시작된 이후 경과된 고정 프레임 시간을 초 단위로 반환합니다. </summary>
		double FixedTimeInSeconds_get()
		{
			return fixedTime;
		}

		/// <summary> (Visual Studio 전용) 이전 프레임에서 경과된 시간을 초 단위로 반환합니다. </summary>
		vs_property_get( double, DeltaTimeInSeconds );

		/// <summary> 이전 프레임에서 경과된 시간을 초 단위로 반환합니다. </summary>
		double DeltaTimeInSeconds_get()
		{
			return deltaTime;
		}

		/// <summary> (Visual Studio 전용) 이전 고정 프레임에서 경과된 시간을 초 단위로 반환합니다. </summary>
		vs_property_get( double, FixedDeltaTimeInSeconds );

		/// <summary> 이전 고정 프레임에서 경과된 시간을 초 단위로 반환합니다. </summary>
		double FixedDeltaTimeInSeconds_get()
		{
			return fixedDeltaTime;
		}

		Time& operator=( const Time& copy )
		{
			time = copy.time;
			fixedTime = copy.fixedTime;
			deltaTime = copy.deltaTime;
			fixedDeltaTime = copy.fixedDeltaTime;
			return *this;
		}
	};
}