#pragma once

namespace SC
{
	/// <summary> 이벤트 데이터를 포함하는 클래스의 기본 클래스를 나타내며, 이벤트 데이터를 포함하지 않는 이벤트에 사용할 값을 제공합니다. </summary>
	class EventArgs : virtual public Object
	{
	public:
		/// <summary> <see cref="EventArgs"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		EventArgs();

		/// <summary> (<see cref="SC::Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;
	};
}