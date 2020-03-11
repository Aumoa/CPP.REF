#pragma once

namespace SC::Drawing
{
	/// <summary>
	/// 진입퇴출 복합 함수를 사용하는 Easing 함수를 계산합니다.
	/// </summary>
	class EaseInOut : public EaseFunction
	{
	protected:
		/// <summary>
		/// (<see cref="EaseFunction"/> 클래스에서 상속 됨.) 진입퇴출 복합 함수를 사용하여 함수를 계산합니다.
		/// </summary>
		/// <param name="T"> 시간값을 전달받습니다. </param>
		/// <returns> 결과값을 반환합니다. </returns>
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
		/// <see cref="EaseInOut"/> 클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="duration"> 함수의 재생 시간을 전달합니다. </param>
		EaseInOut( double duration )
			: EaseFunction( duration )
		{

		}
	};
}