#pragma once

namespace SC::Game
{
	/// <summary> ���� �������� �ð� ������ ���� ����ϴ� �� �����Դϴ�. </summary>
	struct Time : public ValueType
	{
		friend class Scene;

	private:
		double time;
		double fixedTime;
		double deltaTime;
		double fixedDeltaTime;

	public:
		/// <summary> <see cref="Time"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Time()
			: time( 0 )
			, fixedTime( 0 )
			, deltaTime( 0 )
			, fixedDeltaTime( 0 )
		{

		}

		/// <summary> <see cref="Time"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Time( const Time& copy )
			: time( copy.time )
			, fixedTime( copy.fixedTime )
			, deltaTime( copy.deltaTime )
			, fixedDeltaTime( copy.fixedDeltaTime )
		{

		}

		/// <summary> (Visual Studio ����) ����� ���۵� ���� ����� �ð��� �� ������ ��ȯ�մϴ�. </summary>
		vs_property_get( double, TimeInSeconds );

		/// <summary> ����� ���۵� ���� ����� �ð��� �� ������ ��ȯ�մϴ�. </summary>
		double TimeInSeconds_get()
		{
			return time;
		}

		/// <summary> (Visual Studio ����) ����� ���۵� ���� ����� ���� ������ �ð��� �� ������ ��ȯ�մϴ�. </summary>
		vs_property_get( double, FixedTimeInSeconds );

		/// <summary> ����� ���۵� ���� ����� ���� ������ �ð��� �� ������ ��ȯ�մϴ�. </summary>
		double FixedTimeInSeconds_get()
		{
			return fixedTime;
		}

		/// <summary> (Visual Studio ����) ���� �����ӿ��� ����� �ð��� �� ������ ��ȯ�մϴ�. </summary>
		vs_property_get( double, DeltaTimeInSeconds );

		/// <summary> ���� �����ӿ��� ����� �ð��� �� ������ ��ȯ�մϴ�. </summary>
		double DeltaTimeInSeconds_get()
		{
			return deltaTime;
		}

		/// <summary> (Visual Studio ����) ���� ���� �����ӿ��� ����� �ð��� �� ������ ��ȯ�մϴ�. </summary>
		vs_property_get( double, FixedDeltaTimeInSeconds );

		/// <summary> ���� ���� �����ӿ��� ����� �ð��� �� ������ ��ȯ�մϴ�. </summary>
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