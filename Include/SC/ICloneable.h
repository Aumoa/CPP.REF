#pragma once

namespace SC
{
	/// <summary> 같은 값을 사용하여 새 인스턴스를 만드는 복제 함수에 대한 지원을 제공합니다. </summary>
	struct ICloneable : virtual public Object
	{
		/// <summary> 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		virtual object Clone() = 0;
	};
}