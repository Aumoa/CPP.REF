#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// ���� �Լ��� ����ϴ� Easing �Լ��� ����մϴ�.
	/// </summary>
	class EaseIn : public EaseFunction
	{
	protected:
		/// <summary>
		/// (<see cref="EaseFunction"/> Ŭ�������� ��� ��.) ���� �Լ��� ����Ͽ� �Լ��� ����մϴ�.
		/// </summary>
		/// <param name="T"> �ð����� ���޹޽��ϴ�. </param>
		/// <returns> ������� ��ȯ�մϴ�. </returns>
		double Compute( double T ) override
		{
			return T * T;
		}

	public:
		/// <summary>
		/// <see cref="EaseIn"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
		/// </summary>
		/// <param name="duration"> �Լ��� ��� �ð��� �����մϴ�. </param>
		EaseIn( double duration ) : EaseFunction( duration )
		{

		}
	};
}