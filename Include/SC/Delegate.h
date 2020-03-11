#pragma once

namespace SC
{
	/// <summary> 정적 메서드 또는 클래스 인스턴스 및 해당 클래스의 인스턴스 메서드를 참조하는 데이터 구조입니다. </summary>
	struct Delegate : public ValueType
	{
	protected:
		/// <summary> <see cref="Delegate"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Delegate() = default;

	public:
		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;
	};
}