#pragma once

namespace SC
{
	/// <summary> SC 클래스 형식 계층 구조의 루트입니다. 파생 클래스에 하위 수준 기본 서비스를 제공합니다. </summary>
	class Object
	{
		template< class T > friend class RefPtr;
		friend struct WeakPtr;
		friend struct Details::ReferenceCounter;

		static std::atomic<int> instanceCount;

		Details::ReferenceCounter* counter = nullptr;
		bool locked = false;
		
	private:
		Object( int );
		Object( const Object& ) = delete;
		Object& operator=( const Object& ) = delete;

		size_t AddRef();
		size_t Release();

	public:
		/// <summary> <see cref="Object"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Object();
		virtual ~Object();

		/// <summary> 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		virtual String ToString();

		/// <summary> 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		virtual bool Equals( object obj );

		/// <summary> (Visual Studio 전용) 개체의 수명 관리 상태를 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsLocked );

		/// <summary> 개체의 수명 관리 상태를 나타내는 값을 가져옵니다. </summary>
		bool IsLocked_get();

		/// <summary> 개체의 수명 관리 상태를 나타내는 값을 설정합니다. </summary>
		void IsLocked_set( bool value );

		/// <summary> 지정한 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		/// <param name="obj"> 개체를 전달합니다. </param>
		static String ToString( object obj );

		/// <summary> 지정한 개체 인스턴스가 동일한지 확인합니다. </summary>
		/// <param name="objA"> 비교할 첫 번째 개체를 전달합니다. 개체가 null이 아닐 경우 개체의 재정의 함수 Equals가 호출됩니다. </param>
		/// <param name="objB"> 비교할 두 번째 개체를 전달합니다. </param>
		static bool Equals( object objA, object objB );

		/// <summary> 지정한 <see cref="Object"/> 인스턴스가 동일한지 여부를 확인합니다. </summary>
		/// <param name="objA"> 비교할 첫 번째 개체를 전달합니다. 개체가 null이 아닐 경우 개체의 재정의 함수 Equals가 호출됩니다. </param>
		/// <param name="objB"> 비교할 두 번째 개체를 전달합니다. </param>
		static bool ReferenceEquals( object objA, object objB );

		/// <summary> 이 클래스를 상속받는 모든 클래스의 인스턴스 개수를 가져옵니다. </summary>
		static int GetInstanceCount();

		bool operator==( object right );
		bool operator!=( object right );
	};
}