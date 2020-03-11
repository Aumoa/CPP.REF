#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// Easing �Լ��� ����� �ִϸ��̼� Ű�������� ������Ʈ�մϴ�.
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
		/// �Լ��� �ð����� ������ ����մϴ�.
		/// </summary>
		/// <param name="T"> �ð����� ���޹޽��ϴ�. </param>
		/// <returns> ������� ��ȯ�մϴ�. </returns>
		virtual double Compute( double T ) = 0;

	public:
		/// <summary>
		/// <see cref="EaseFunction"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
		/// </summary>
		/// <param name="duration"> �Լ��� ��� �ð��� �����մϴ�. </param>
		EaseFunction( double duration )
			: durationInSeconds( duration )
		{

		}

		/// <summary>
		/// ������ Easing �Լ��� �̿��� ���� ��ȯ ���¸� �����մϴ�.
		/// </summary>
		/// <param name="deltaTime"> �ð� �̵����� �����մϴ�. </param>
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
		/// ������ Easing �Լ��� �̿��� ���� ��ȯ ���¸� �����մϴ�. ���� �ð� Ž�� ��ġ�� ����մϴ�.
		/// </summary>
		/// <param name="timeSeek"> ���� �ð� ��ġ�� �����մϴ�. </param>
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
		/// Easing �Լ��� ������ �����մϴ�. �ð� ��ġ�� �������� �����մϴ�.
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
		/// Easing �Լ��� ������ �����մϴ�. �ð� ��ġ�� ���������� �����մϴ�.
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
		/// (Visual Studio ����) �Լ��� ������ ���۵Ǿ����� �˻��մϴ�.
		/// </summary>
		vs_property( bool, IsStart );

		/// <summary>
		/// �Լ��� ������ ���۵Ǿ����� �˻��մϴ�.
		/// </summary>
		bool IsStart_get()
		{
			return isStart;
		}

		/// <summary>
		/// (Visual Studio ����) �Լ��� ���� �����ؼ� �����ɴϴ�.
		/// </summary>
		vs_property( bool, IsInvert );

		/// <summary>
		/// �Լ��� ���� �����ؼ� �����ɴϴ�.
		/// </summary>
		bool IsInvert_get()
		{
			return isInvert;
		}

		/// <summary>
		/// �Լ��� ���� �����ؼ� �����ɴϴ�.
		/// </summary>
		void IsInvert_set( bool value )
		{
			isInvert = value;
		}

		/// <summary>
		/// (Visual Studio ����) �Լ��� ���� ��ġ�� �����ɴϴ�. 0���� 1������ ���� �����ϴ�.
		/// </summary>
		vs_property( double, CurrentT );

		/// <summary>
		/// �Լ��� ���� ��ġ�� �����ɴϴ�. 0���� 1������ ���� �����ϴ�.
		/// </summary>
		double CurrentT_get()
		{
			return currentT;
		}

		/// <summary>
		/// (Visual Studio ����) �Լ��� Easing ���� ���� �����ɴϴ�.
		/// </summary>
		vs_property( double, CurrentValue );

		/// <summary>
		/// �Լ��� Easing ���� ���� �����ɴϴ�.
		/// </summary>
		double CurrentValue_get()
		{
			return currentValue;
		}

		/// <summary>
		/// (Visual Studio ����) �Լ��� ��� �ð��� �ʴ����� �����ɴϴ�.
		/// </summary>
		vs_property( double, DurationInSeconds );

		/// <summary>
		/// �Լ��� ��� �ð��� �ʴ����� �����ɴϴ�.
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