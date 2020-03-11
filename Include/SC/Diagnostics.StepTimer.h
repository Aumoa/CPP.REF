#pragma once

namespace SC::Diagnostics
{
	/// <summary> ��� �ð� �� �ù����̼� Ÿ�̹��� �����ϴ� �� ���Ǵ� Ÿ�̸� ����� �����մϴ�. </summary>
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
		/// <summary> <see cref="StepTimer"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		StepTimer();

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ��� �ð� �� �ʴ� ������ Ƚ���� �ؽ�Ʈ �������� �����ɴϴ�. </summary>
		String ToString() override;

		/// <summary> �ǵ����� Ÿ�̹� ���� �� ������ �ð� ������� �缳���մϴ�. </summary>
		void ResetElapsedTime();

		/// <summary> Ÿ�̸� ���� ����Ʈ�� �����ϰ� Ÿ�̸Ӹ� �����մϴ�. </summary>
		/// <param name="updateCallback"> ������ ȣ�� ���� �Լ��� �����մϴ�. </param>
		void Tick( StepTimerCallbackDelegate updateCallback = StepTimerCallbackDelegate() );

		/// <summary> Ÿ�̸� ���� ����Ʈ�� �����ϰ� Ÿ�̸Ӹ� �����մϴ�. </summary>
		/// <param name="updateCallback"> ������ ȣ�� ���� �Լ��� �����մϴ�. </param>
		void Tick( std::function<void()> updateCallback );

		/// <summary> ���� ���� �ð� ���� �ʴ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		static constexpr const uint64 TicksFrequency = frequency;

		/// <summary> (Visual Studio ����) ���� Tick �Լ� ȣ�� ���� ����� �ð��� �����ɴϴ�. </summary>
		vs_property_get( uint64, ElapsedTicks );

		/// <summary> ���� Tick> �Լ� ȣ�� ���� ����� �ð��� �����ɴϴ�. </summary>
		uint64 ElapsedTicks_get() const;

		/// <summary> (Visual Studio ����) ���� Tick �Լ� ȣ�� ���� ����� �ð��� �����ɴϴ�. </summary>
		vs_property_get( double, ElapsedSeconds );

		/// <summary> ���� Tick �Լ� ȣ�� ���� ����� �ð��� �����ɴϴ�. </summary>
		double ElapsedSeconds_get() const;

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ���۵� ������ �� �ð��� �����ɴϴ�. </summary>
		vs_property_get( uint64, TotalTicks );

		/// <summary> Ÿ�̸Ӱ� ���۵� ������ �� �ð��� �����ɴϴ�. </summary>
		uint64 TotalTicks_get() const;

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ���۵� ������ �� �ð��� �����ɴϴ�. </summary>
		vs_property_get( double, TotalSeconds );

		/// <summary> Ÿ�̸Ӱ� ���۵� ������ �� �ð��� �����ɴϴ�. </summary>
		double TotalSeconds_get() const;

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ���۵� ���� Tick �Լ��� ȣ�� Ƚ���� �����ɴϴ�. </summary>
		vs_property_get( uint64, TickCount );

		/// <summary> Ÿ�̸Ӱ� ���۵� ���� Tick �Լ��� ȣ�� Ƚ���� �����ɴϴ�. </summary>
		uint64 TickCount_get() const;

		/// <summary> (Visual Studio ����) ���� �ʴ� Tick �Լ��� ȣ�� Ƚ���� �����ɴϴ�. </summary>
		vs_property_get( int, TicksPerSecond );

		/// <summary> ���� �ʴ� Tick �Լ��� ȣ�� Ƚ���� �����ɴϴ�. </summary>
		int TicksPerSecond_get() const;

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ������ �ð� ������ ����ϴ��� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsFixedTimeStep );

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����ϴ��� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsFixedTimeStep_get() const;

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����ϴ��� ��Ÿ���� ���� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void IsFixedTimeStep_set( bool value );

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( uint64, TargetElapsedTicks );

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����ɴϴ�. </summary>
		uint64 TargetElapsedTicks_get() const;

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void TargetElapsedTicks_set( uint64 value );

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, TargetElapsedSeconds );

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����ɴϴ�. </summary>
		double TargetElapsedSeconds_get() const;

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void TargetElapsedSeconds_set( double value );

		/// <summary> (Visual Studio ����) Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, TargetElapsedTicksPerSecond );

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����ɴϴ�. </summary>
		int TargetElapsedTicksPerSecond_get() const;

		/// <summary> Ÿ�̸Ӱ� ������ �ð� ������ ����� �� ȣ�� �󵵸� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void TargetElapsedTicksPerSecond_set( int value );

		/// <summary> (Visual Studio ����) ������ �̵� �ð� ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( uint64, MaxDeltaTicks );

		/// <summary> ������ �̵� �ð� ������ �����ɴϴ�. </summary>
		uint64 MaxDeltaTicks_get() const;

		/// <summary> ������ �̵� �ð� ������ �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void MaxDeltaTicks_set( uint64 value );

		/// <summary> (Visual Studio ����) ������ �̵� �ð� ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, MaxDeltaSeconds );

		/// <summary> ������ �̵� �ð� ������ �����ɴϴ�. </summary>
		double MaxDeltaSeconds_get() const;

		/// <summary> ������ �̵� �ð� ������ �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void MaxDeltaSeconds_set( double value );

	private:
		uint64 GetNanos();
		static double TicksToSeconds( uint64 ticks );
		static uint64 SecondsToTicks( double seconds );
	};
}