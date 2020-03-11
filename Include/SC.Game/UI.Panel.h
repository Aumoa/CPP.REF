#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 요소를 정리하는 패널 형식 계층 구조의 루트입니다. 파생 클래스에 패널 형식의 기본 기능을 제공합니다. </summary>
	class Panel : public Element, virtual public Collections::IList<RefPtr<Element>>
	{
		class Enumerator;

	public:
		using ContentType = std::shared_ptr<std::vector<RefPtr<Element>>>;

	private:
		ContentType contents;

	protected:
		/// <summary> <see cref="Panel"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		Panel( String name );

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> (<see cref="IEnumerable"/> 인터페이스에서 구현 됨.) 컬렉션 전체를 반복하는 열거자를 반환합니다. </summary>
		RefPtr<IEnumerator<RefPtr<Element>>> GetEnumerator() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 항목을 추가합니다. </summary>
		/// <param name="value"> 추가할 개체를 전달합니다. </param>
		/// <returns> 새 요소를 삽입한 위치를 반환합니다. 실패하였을 경우 -1을 반환합니다. </returns>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		int Add( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 항목을 모두 제거합니다. </summary>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void Clear() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 특정 항목이 존재하는지 여부를 확인합니다. </summary>
		/// <param name="value"> 찾을 개체를 전달합니다. </param>
		/// <returns> 개체가 리스트에 있으면 true, 그렇지 않다면 false를 반환합니다. </returns>
		bool Contains( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 특정 인덱스부터 시작하여 요소를 지정한 배열에 복사합니다. </summary>
		/// <param name="array"> 복사 대상 배열 포인터입니다. 충분한 크기가 준비되어야 합니다. </param>
		/// <param name="index"> 복사가 시작되는 인덱스입니다. </param>
		/// <exception cref="ArgumentNullException"> <paramref name="array"/>가 nullptr인 경우 발생합니다. </exception>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 크거나 같을 경우 경우 발생합니다. </exception>
		void CopyTo( RefPtr<Element>* array, int index = 0 ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 특정 항목의 인덱스를 검색합니다. </summary>
		/// <param name="value"> 찾을 개체를 전달합니다. </param>
		/// <returns> 목록에 개체가 존재하면 개체의 인덱스를, 그렇지 않다면 -1을 반환합니다. </returns>
		int IndexOf( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 리스트에 새 요소를 지정한 인덱스에 삽입합니다. </summary>
		/// <param name="index"> 삽입할 인덱스를 전달합니다. </param>
		/// <param name="value"> 삽입할 개체를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 클 경우 경우 발생합니다. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void Insert( int index, RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 리스트에서 제일 처음 발견되는 특정 개체를 제거합니다. </summary>
		/// <param name="value"> 제거할 개체를 전달합니다. </param>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void Remove( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 리스트에서 제일 처음 발견되는 특정 개체를 제거합니다. </summary>
		/// <param name="index"> 제거할 항목의 인덱스를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 크거나 같을 경우 경우 발생합니다. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void RemoveAt( int index ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) UI 이벤트를 처리합니다. </summary>
		/// <param name="args"> 이벤트 매개변수가 전달됩니다. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> 특정 항목의 인덱스를 검색합니다. </summary>
		/// <param name="name"> 찾을 개체의 이름을 전달합니다. </param>
		/// <returns> 목록에 개체가 존재하면 개체의 인덱스를, 그렇지 않다면 -1을 반환합니다. </returns>
		int IndexOf( String name );

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 포함된 요소 수를 가져옵니다. </summary>
		int Count_get() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 크기가 고정되어 있는지 여부를 나타내는 값을 가져옵니다. </summary>
		bool IsFixedSize_get() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 지정한 인덱스에 있는 요소를 가져옵니다. </summary>
		RefPtr<Element> Item_get( int index ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 지정한 인덱스에 있는 요소를 설정합니다. </summary>
		void Item_set( int index, RefPtr<Element> value ) override;

		/// <summary> 이 클래스의 이름 해시 값을 가져옵니다. </summary>
		static const std::size_t ClassName;

		/// <summary> 패널에 포함된 요소의 Z 순서를 지정하는 속성의 해시 값을 가져옵니다. </summary>
		static const std::size_t ZOrder;
	};
}