#pragma once

namespace SC::Game::UI
{
	/// <summary> 고정 위치를 사용하여 요소의 위치를 정리하는 패널 형식입니다. </summary>
	class FixedPanel : public Panel
	{
	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

	public:
		/// <summary> <see cref="FixedPanel"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		FixedPanel( String name );

		/// <summary> 이 클래스의 이름 해시 값을 가져옵니다. </summary>
		static const std::size_t ClassName;

		/// <summary> 고정 패널의 왼쪽 위치를 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t Left;

		/// <summary> 고정 패널의 위쪽 위치를 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t Top;

		/// <summary> 고정 패널의 오른쪽 위치를 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t Right;

		/// <summary> 고정 패널의 아래쪽 위치를 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t Bottom;
	};
}