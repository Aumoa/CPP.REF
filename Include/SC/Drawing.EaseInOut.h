#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// �������� ���� �Լ��� ����ϴ� Easing �Լ��� ����մϴ�.
	/// </summary>
	class EaseInOut : public EaseFunction
	{
	protected:
		/// <summary>
		/// (<see cref="EaseFunction"/> Ŭ�������� ��� ��.) �������� ���� �Լ��� ����Ͽ� �Լ��� ����մϴ�.
		/// </summary>
		/// <param name="T"> �ð����� ���޹޽��ϴ�. </param>
		/// <returns> ������� ��ȯ�մϴ�. </returns>
		double Compute( double T ) override
		{
			T *= 2.0;
			if ( T < 1.0 ) return 0.5 * T * T;
			else
			{
				T -= 1.0;
				return -0.5 * ( T * ( T - 2.0 ) - 1.0 );
			}
		}

	public:
		/// <summary>
		/// <see cref="EaseInOut"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
		/// </summary>
		/// <param name="duration"> �Լ��� ��� �ð��� �����մϴ�. </param>
		EaseInOut( double duration )
			: EaseFunction( duration )
		{

		}
	};
}