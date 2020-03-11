#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// Easing 함수를 계산해 애니메이션 키프레임을 업데이트합니다.
	/// </summary>
	class EaseFunction : virtual public Object
	{
		double timeSeek = 0.0f;
		bool isStart = false;
		bool isInvert = false;
		double currentT = 0.0;
		double currentValue = 0.0;
		const double durationInSeconds;

	protected:
		/// <summary>
		/// 함수를 시간값에 기초해 계산합니다.
		/// </summary>
		/// <param name="T"> 시간값을 전달받습니다. </param>
		/// <returns> 결과값을 반환합니다. </returns>
		virtual double Compute( double T ) = 0;

	public:
		/// <summary>
		/// <see cref="EaseFunction"/> 클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="duration"> 함수의 재생 시간을 전달합니다. </param>
		EaseFunction( double duration )
			: durationInSeconds( duration )
		{

		}

		/// <summary>
		/// 지정된 Easing 함수를 이용해 현재 변환 상태를 갱신합니다.
		/// </summary>
		/// <param name="deltaTime"> 시간 이동값을 전달합니다. </param>
		void Update( double deltaTime )
		{
			if ( IsStart_get() )
			{
				timeSeek += deltaTime;

				if ( timeSeek >= DurationInSeconds_get() )
				{
					timeSeek = DurationInSeconds_get();
					IsStart_set( false );
				}

				double t = timeSeek / DurationInSeconds_get();
				auto v = Compute( t );
				if ( IsInvert_get() ) CurrentValue_set( 1.0 - v );
				else CurrentValue_set( v );
			}
		}

		/// <summary>
		/// 지정된 Easing 함수를 이용해 현재 변환 상태를 갱신합니다. 고정 시간 탐색 위치를 사용합니다.
		/// </summary>
		/// <param name="timeSeek"> 고정 시간 위치를 전달합니다. </param>
		void FixedUpdate( double timeSeek )
		{
			if ( IsStart_get() )
			{
				if ( timeSeek >= DurationInSeconds_get() )
				{
					timeSeek = DurationInSeconds_get();
					IsStart_set( false );
				}

				double t = timeSeek / DurationInSeconds_get();
				auto v = Compute( t );
				if ( IsInvert_get() ) CurrentValue_set( 1.0 - v );
				else CurrentValue_set( v );
			}
		}

		/// <summary>
		/// Easing 함수의 보간을 시작합니다. 시간 위치를 시작으로 설정합니다.
		/// </summary>
		void Start()
		{
			if ( IsStart_get() == false )
			{
				IsStart_set( true );
			}

			timeSeek = 0;
			CurrentValue_set( Compute( 0.0 ) );
		}

		/// <summary>
		/// Easing 함수의 보간을 종료합니다. 시간 위치를 마지막으로 설정합니다.
		/// </summary>
		void Stop()
		{
			if ( IsStart_get() )
			{
				IsStart_set( false );
			}

			timeSeek = DurationInSeconds_get();
			CurrentValue_set( Compute( 1.0 ) );
		}

		/// <summary>
		/// (Visual Studio 전용) 함수의 보간이 시작되었는지 검사합니다.
		/// </summary>
		vs_property( bool, IsStart );

		/// <summary>
		/// 함수의 보간이 시작되었는지 검사합니다.
		/// </summary>
		bool IsStart_get()
		{
			return isStart;
		}

		/// <summary>
		/// (Visual Studio 전용) 함수의 값을 반전해서 가져옵니다.
		/// </summary>
		vs_property( bool, IsInvert );

		/// <summary>
		/// 함수의 값을 반전해서 가져옵니다.
		/// </summary>
		bool IsInvert_get()
		{
			return isInvert;
		}

		/// <summary>
		/// 함수의 값을 반전해서 가져옵니다.
		/// </summary>
		void IsInvert_set( bool value )
		{
			isInvert = value;
		}

		/// <summary>
		/// (Visual Studio 전용) 함수의 보간 위치를 가져옵니다. 0에서 1사이의 값을 가집니다.
		/// </summary>
		vs_property( double, CurrentT );

		/// <summary>
		/// 함수의 보간 위치를 가져옵니다. 0에서 1사이의 값을 가집니다.
		/// </summary>
		double CurrentT_get()
		{
			return currentT;
		}

		/// <summary>
		/// (Visual Studio 전용) 함수의 Easing 보간 값을 가져옵니다.
		/// </summary>
		vs_property( double, CurrentValue );

		/// <summary>
		/// 함수의 Easing 보간 값을 가져옵니다.
		/// </summary>
		double CurrentValue_get()
		{
			return currentValue;
		}

		/// <summary>
		/// (Visual Studio 전용) 함수의 재생 시간을 초단위로 가져옵니다.
		/// </summary>
		vs_property( double, DurationInSeconds );

		/// <summary>
		/// 함수의 재생 시간을 초단위로 가져옵니다.
		/// </summary>
		double DurationInSeconds_get()
		{
			return durationInSeconds;
		}

	private:
		void IsStart_set( bool value )
		{
			isStart = value;
		}

		void CurrentT_set( double value )
		{
			currentT = value;
		}

		void CurrentValue_set( double value )
		{
			currentValue = value;
		}
	};
}