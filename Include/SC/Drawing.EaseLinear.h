#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// ���� ������ ����ϴ� Easing �Լ��� ����մϴ�.
	/// </summary>
	class EaseLinear : public EaseFunction
	{
	protected:
		/// <summary>
		/// (<see cref="EaseFunction"/> Ŭ�������� ��� ��.) ���� ������ ����Ͽ� �Լ��� ����մϴ�.
		/// </summary>
		/// <param name="T"> �ð����� ���޹޽��ϴ�. </param>
		/// <returns> ������� ��ȯ�մϴ�. </returns>
		double Compute( double T ) override
		{
			return T;
		}

	public:
		/// <summary>
		/// <see cref="EaseLinear"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
		/// </summary>
		/// <param name="duration"> �Լ��� ��� �ð��� �����մϴ�. </param>
		EaseLinear( double duration ) : EaseFunction( duration )
		{

		}
	};
}