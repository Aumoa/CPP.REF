#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// ���� �Լ��� ����ϴ� Easing �Լ��� ����մϴ�.
	/// </summary>
	class EaseOut : public EaseFunction
	{
	protected:
		/// <summary>
		/// (<see cref="EaseFunction"/> Ŭ�������� ��� ��.) ���� �Լ��� ����Ͽ� �Լ��� ����մϴ�.
		/// </summary>
		/// <param name="T"> �ð����� ���޹޽��ϴ�. </param>
		/// <returns> ������� ��ȯ�մϴ�. </returns>
		double Compute( double T ) override
		{
			return 1.0 * T * ( T - 2.0 );
		}

	public:
		/// <summary>
		/// <see cref="EaseOut"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
		/// </summary>
		/// <param name="duration"> �Լ��� ��� �ð��� �����մϴ�. </param>
		EaseOut( double duration ) : EaseFunction( duration )
		{

		}
	};
}