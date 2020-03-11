#pragma once

namespace SC::Collections
{
	/// <summary> 인덱스에서 개별적으로 액세스할 수 있는 개체의 컬렉션을 나타냅니다. </summary>
	template< class T >
	struct IList : virtual public Object, virtual public IEnumerable<T>
	{
		/// <summary> 항목을 추가합니다. </summary>
		/// <param name="value"> 추가할 개체를 전달합니다. </param>
		/// <returns> 새 요소를 삽입한 위치를 반환합니다. 실패하였을 경우 -1을 반환합니다. </returns>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		virtual int Add( T value ) = 0;

		/// <summary> 항목을 모두 제거합니다. </summary>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		virtual void Clear() = 0;

		/// <summary> 특정 항목이 존재하는지 여부를 확인합니다. </summary>
		/// <param name="value"> 찾을 개체를 전달합니다. </param>
		/// <returns> 개체가 리스트에 있으면 true, 그렇지 않다면 false를 반환합니다. </returns>
		virtual bool Contains( T value ) = 0;

		/// <summary> 특정 인덱스부터 시작하여 요소를 지정한 배열에 복사합니다. </summary>
		/// <param name="array"> 복사 대상 배열 포인터입니다. 충분한 크기가 준비되어야 합니다. </param>
		/// <param name="index"> 복사가 시작되는 인덱스입니다. </param>
		/// <exception cref="ArgumentNullException"> <paramref name="array"/>가 nullptr인 경우 발생합니다. </exception>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 크거나 같을 경우 경우 발생합니다. </exception>
		virtual void CopyTo( T* array, int index = 0 ) = 0;

		/// <summary> 특정 항목의 인덱스를 검색합니다. </summary>
		/// <param name="value"> 찾을 개체를 전달합니다. </param>
		/// <returns> 목록에 개체가 존재하면 개체의 인덱스를, 그렇지 않다면 -1을 반환합니다. </returns>
		virtual int IndexOf( T value ) = 0;

		/// <summary> 리스트에 새 요소를 지정한 인덱스에 삽입합니다. </summary>
		/// <param name="index"> 삽입할 인덱스를 전달합니다. </param>
		/// <param name="value"> 삽입할 개체를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 클 경우 경우 발생합니다. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		virtual void Insert( int index, T value ) = 0;

		/// <summary> 리스트에서 제일 처음 발견되는 특정 개체를 제거합니다. </summary>
		/// <param name="value"> 제거할 개체를 전달합니다. </param>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		virtual void Remove( T value ) = 0;

		/// <summary> 리스트에서 제일 처음 발견되는 특정 개체를 제거합니다. </summary>
		/// <param name="index"> 제거할 항목의 인덱스를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 크거나 같을 경우 경우 발생합니다. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		virtual void RemoveAt( int index ) = 0;

		/// <summary> (Visual Studio 전용) 포함된 요소 수를 가져옵니다. </summary>
		vs_property_get( int, Count );

		/// <summary> 포함된 요소 수를 가져옵니다. </summary>
		virtual int Count_get() = 0;

		/// <summary> (Visual Studio 전용) 크기가 고정되어 있는지 여부를 나타내는 값을 가져옵니다. </summary>
		vs_property_get( bool, IsFixedSize );

		/// <summary> 크기가 고정되어 있는지 여부를 나타내는 값을 가져옵니다. </summary>
		virtual bool IsFixedSize_get() = 0;

		/// <summary> (Visual Studio 전용) 지정한 인덱스에 있는 요소를 가져오거나 설정합니다. </summary>
		vs_property( T, Item )[];

		/// <summary> 지정한 인덱스에 있는 요소를 가져옵니다. </summary>
		virtual T Item_get( int index ) = 0;

		/// <summary> 지정한 인덱스에 있는 요소를 설정합니다. </summary>
		virtual void Item_set( int index, T value ) = 0;
	};
}