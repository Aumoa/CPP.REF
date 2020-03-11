#pragma once

namespace SC::Diagnostics
{
	/// <summary> 경과 시간 및 시물레이션 타이밍을 측정하는 데 사용되는 타이머 기능을 제공합니다. </summary>
	class StepTimer : virtual public Object
	{
		static constexpr const uint64 frequency = 1'000'000'000;

		uint64 nanoStandard = 0;

		uint64 nanoLasttime = 0;
		uint64 nanoMaxDelta = 0;

		uint64 elapsedTicks = 0;
		uint64 totalTicks = 0;
		uint64 leftOverTicks = 0;

		uint32 frameCount = 0;
		uint32 framesPerSecond = 0;
		uint32 framesThisSecond = 0;
		uint64 nanoSecondCounter = 0;

		bool isFixedTimeStep = false;
		uint64 targetElapsedTicks = 0;

	public:
		/// <summary> <see cref="StepTimer"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		StepTimer();

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 경과 시간 및 초당 프레임 횟수를 텍스트 형식으로 가져옵니다. </summary>
		String ToString() override;

		/// <summary> 의도적인 타이밍 중지 후 프레임 시간 경과값을 재설정합니다. </summary>
		void ResetElapsedTime();

		/// <summary> 타이머 기준 포인트를 설정하고 타이머를 갱신합니다. </summary>
		/// <param name="updateCallback"> 프레임 호출 갱신 함수를 제공합니다. </param>
		void Tick( StepTimerCallbackDelegate updateCallback = StepTimerCallbackDelegate() );

		/// <summary> 타이머 기준 포인트를 설정하고 타이머를 갱신합니다. </summary>
		/// <param name="updateCallback"> 프레임 호출 갱신 함수를 제공합니다. </param>
		void Tick( std::function<void()> updateCallback );

		/// <summary> 정수 형식 시간 값이 초당 나타내는 값을 가져옵니다. </summary>
		static constexpr const uint64 TicksFrequency = frequency;

		/// <summary> (Visual Studio 전용) 이전 Tick 함수 호출 이후 경과한 시간을 가져옵니다. </summary>
		vs_property_get( uint64, ElapsedTicks );

		/// <summary> 이전 Tick> 함수 호출 이후 경과한 시간을 가져옵니다. </summary>
		uint64 ElapsedTicks_get() const;

		/// <summary> (Visual Studio 전용) 이전 Tick 함수 호출 이후 경과한 시간을 가져옵니다. </summary>
		vs_property_get( double, ElapsedSeconds );

		/// <summary> 이전 Tick 함수 호출 이후 경과한 시간을 가져옵니다. </summary>
		double ElapsedSeconds_get() const;

		/// <summary> (Visual Studio 전용) 타이머가 시작된 이후의 총 시간을 가져옵니다. </summary>
		vs_property_get( uint64, TotalTicks );

		/// <summary> 타이머가 시작된 이후의 총 시간을 가져옵니다. </summary>
		uint64 TotalTicks_get() const;

		/// <summary> (Visual Studio 전용) 타이머가 시작된 이후의 총 시간을 가져옵니다. </summary>
		vs_property_get( double, TotalSeconds );

		/// <summary> 타이머가 시작된 이후의 총 시간을 가져옵니다. </summary>
		double TotalSeconds_get() const;

		/// <summary> (Visual Studio 전용) 타이머가 시작된 이후 Tick 함수의 호출 횟수를 가져옵니다. </summary>
		vs_property_get( uint64, TickCount );

		/// <summary> 타이머가 시작된 이후 Tick 함수의 호출 횟수를 가져옵니다. </summary>
		uint64 TickCount_get() const;

		/// <summary> (Visual Studio 전용) 현재 초당 Tick 함수의 호출 횟수를 가져옵니다. </summary>
		vs_property_get( int, TicksPerSecond );

		/// <summary> 현재 초당 Tick 함수의 호출 횟수를 가져옵니다. </summary>
		int TicksPerSecond_get() const;

		/// <summary> (Visual Studio 전용) 타이머가 고정된 시간 비율을 사용하는지 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsFixedTimeStep );

		/// <summary> 타이머가 고정된 시간 비율을 사용하는지 나타내는 값을 가져옵니다. </summary>
		bool IsFixedTimeStep_get() const;

		/// <summary> 타이머가 고정된 시간 비율을 사용하는지 나타내는 값을 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void IsFixedTimeStep_set( bool value );

		/// <summary> (Visual Studio 전용) 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 설정하거나 가져옵니다. </summary>
		vs_property( uint64, TargetElapsedTicks );

		/// <summary> 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 가져옵니다. </summary>
		uint64 TargetElapsedTicks_get() const;

		/// <summary> 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void TargetElapsedTicks_set( uint64 value );

		/// <summary> (Visual Studio 전용) 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 설정하거나 가져옵니다. </summary>
		vs_property( double, TargetElapsedSeconds );

		/// <summary> 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 가져옵니다. </summary>
		double TargetElapsedSeconds_get() const;

		/// <summary> 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void TargetElapsedSeconds_set( double value );

		/// <summary> (Visual Studio 전용) 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 설정하거나 가져옵니다. </summary>
		vs_property( int, TargetElapsedTicksPerSecond );

		/// <summary> 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 가져옵니다. </summary>
		int TargetElapsedTicksPerSecond_get() const;

		/// <summary> 타이머가 고정된 시간 비율을 사용할 때 호출 빈도를 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void TargetElapsedTicksPerSecond_set( int value );

		/// <summary> (Visual Studio 전용) 프레임 이동 시간 상한을 설정하거나 가져옵니다. </summary>
		vs_property( uint64, MaxDeltaTicks );

		/// <summary> 프레임 이동 시간 상한을 가져옵니다. </summary>
		uint64 MaxDeltaTicks_get() const;

		/// <summary> 프레임 이동 시간 상한을 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void MaxDeltaTicks_set( uint64 value );

		/// <summary> (Visual Studio 전용) 프레임 이동 시간 상한을 설정하거나 가져옵니다. </summary>
		vs_property( double, MaxDeltaSeconds );

		/// <summary> 프레임 이동 시간 상한을 가져옵니다. </summary>
		double MaxDeltaSeconds_get() const;

		/// <summary> 프레임 이동 시간 상한을 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void MaxDeltaSeconds_set( double value );

	private:
		uint64 GetNanos();
		static double TicksToSeconds( uint64 ticks );
		static uint64 SecondsToTicks( double seconds );
	};
}