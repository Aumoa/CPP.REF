#pragma once

namespace SC::Game::UI
{
	/// <summary> 상호 관계적 위치를 사용하여 요소의 위치를 정리하는 패널 형식입니다. </summary>
	class RelativePanel : public Panel
	{
	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

	public:
		/// <summary> <see cref="RelativePanel"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		RelativePanel( String name );

		/// <summary> 이 클래스의 이름 해시 값을 가져옵니다. </summary>
		static const std::size_t ClassName;

		/// <summary> 관계 패널에 소속된 요소의 왼쪽에 위치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t LeftOf;

		/// <summary> 관계 패널에 소속된 요소의 위쪽에 위치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t Above;

		/// <summary> 관계 패널에 소속된 요소의 오른쪽에 위치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t RightOf;

		/// <summary> 관계 패널에 소속된 요소의 아래에 위치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t Below;

		/// <summary> 관계 패널에 소속된 요소의 왼쪽이 일치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t AlignLeftWith;

		/// <summary> 관계 패널에 소속된 요소의 위쪽이 일치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t AlignTopWith;

		/// <summary> 관계 패널에 소속된 요소의 오른쪽이 일치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t AlignRightWith;

		/// <summary> 관계 패널에 소속된 요소의 아래쪽이 일치하도록 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t AlignBottomWith;

	private:
		void ComputeRect( int index, std::vector<Drawing::Rect<double>>& computedRects, std::vector<bool>& computed );
	};
}