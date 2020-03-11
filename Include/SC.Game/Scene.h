#pragma once

namespace SC::Game
{
	namespace Core
	{
		class GameLogic;
	}

	/// <summary> 게임 장면에 대한 기초 클래스를 표현합니다. </summary>
	class Scene : virtual public Object, virtual public Collections::IList<RefPtr<GameObject>>
	{
		friend class Details::GameLogic;

		std::vector<RefPtr<GameObject>> gameObjects;

		Time time;
		Input input;
		Diagnostics::StepTimer timer;
		Diagnostics::StepTimer fixedTimer;

		bool firstUpdate = false;

	public:
		/// <summary> <see cref="Scene"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Scene();
		~Scene() override;

		/// <summary> (<see cref="IEnumerable"/> 인터페이스에서 구현 됨.) 컬렉션 전체를 반복하는 열거자를 반환합니다. </summary>
		RefPtr<IEnumerator<RefPtr<GameObject>>> GetEnumerator() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 항목을 추가합니다. </summary>
		/// <param name="value"> 추가할 개체를 전달합니다. </param>
		/// <returns> 새 요소를 삽입한 위치를 반환합니다. 실패하였을 경우 -1을 반환합니다. </returns>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		int Add( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 항목을 모두 제거합니다. </summary>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void Clear() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 특정 항목이 존재하는지 여부를 확인합니다. </summary>
		/// <param name="value"> 찾을 개체를 전달합니다. </param>
		/// <returns> 개체가 리스트에 있으면 true, 그렇지 않다면 false를 반환합니다. </returns>
		bool Contains( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 특정 인덱스부터 시작하여 요소를 지정한 배열에 복사합니다. </summary>
		/// <param name="array"> 복사 대상 배열 포인터입니다. 충분한 크기가 준비되어야 합니다. </param>
		/// <param name="index"> 복사가 시작되는 인덱스입니다. </param>
		/// <exception cref="ArgumentNullException"> <paramref name="array"/>가 nullptr인 경우 발생합니다. </exception>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 크거나 같을 경우 경우 발생합니다. </exception>
		void CopyTo( RefPtr<GameObject>* array, int index = 0 ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 특정 항목의 인덱스를 검색합니다. </summary>
		/// <param name="value"> 찾을 개체를 전달합니다. </param>
		/// <returns> 목록에 개체가 존재하면 개체의 인덱스를, 그렇지 않다면 -1을 반환합니다. </returns>
		int IndexOf( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 리스트에 새 요소를 지정한 인덱스에 삽입합니다. </summary>
		/// <param name="index"> 삽입할 인덱스를 전달합니다. </param>
		/// <param name="value"> 삽입할 개체를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 클 경우 경우 발생합니다. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void Insert( int index, RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 리스트에서 제일 처음 발견되는 특정 개체를 제거합니다. </summary>
		/// <param name="value"> 제거할 개체를 전달합니다. </param>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void Remove( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 리스트에서 제일 처음 발견되는 특정 개체를 제거합니다. </summary>
		/// <param name="index"> 제거할 항목의 인덱스를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>가 0보다 작거나 배열의 크기보다 크거나 같을 경우 경우 발생합니다. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> 개체가 읽기 전용 리스트인 경우 발생합니다. </para>
		/// <para> 개체가 고정 크기일 경우 발생합니다. </para>
		/// </exception>
		void RemoveAt( int index ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 포함된 요소 수를 가져옵니다. </summary>
		int Count_get() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 크기가 고정되어 있는지 여부를 나타내는 값을 가져옵니다. </summary>
		bool IsFixedSize_get() override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 지정한 인덱스에 있는 요소를 가져옵니다. </summary>
		RefPtr<GameObject> Item_get( int index ) override;

		/// <summary> (<see cref="Collections::IList"/> 인터페이스에서 구현 됨.) 지정한 인덱스에 있는 요소를 설정합니다. </summary>
		void Item_set( int index, RefPtr<GameObject> value ) override;

		/// <summary> 장면에서 첫 Update가 호출되기 전 실행됩니다. </summary>
		virtual void Start();

		/// <summary> 장면에 포함된 모든 개체에 대한 논리 갱신을 순서대로 수행합니다. </summary>
		virtual void Update();

		/// <summary> 장면에 포함된 모든 개체에 대한 고정 프레임 논리 갱신을 순서대로 수행합니다. </summary>
		virtual void FixedUpdate();

		/// <summary> 장면에 포함된 모든 개체에 대한 늦은 프레임 논리 갱신을 순서대로 수행합니다. </summary>
		 virtual void LateUpdate();

		/// <summary> 장면이 로드될 때 호출되는 함수입니다. </summary>
		/// <param name="asyncLoad"> 장면이 비동기 상태로 로드될 때 비동기 상태 개체가 전달됩니다. </param>
		virtual void Load( RefPtr<IAsyncLoad> asyncLoad );

		/// <summary> 장면이 언로드될 때 호출되는 함수입니다. </summary>
		/// <param name="progress"> 작업의 진행률을 보고할 포인터 변수입니다. </param>
		virtual void Unload();

	private:
		void Render( RefPtr<Details::CDeviceContext>& deviceContext );
	};
}