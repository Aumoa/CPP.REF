#pragma once

namespace SC
{
	/// <summary> ++ 연산자 및 == 연산자를 지원하는 항목에 대한 기초 열거자 형식을 제공합니다. </summary>
	template< class T >
	class BasicEnumerator : virtual public Object, virtual public IEnumerator<T>
	{
		object instance;
		object begin;
		object end;
		object current;
		std::function<object( object )> increment;
		std::function<T( object )> get;
		bool start = false;

	public:
		/// <summary> <see cref="BasicEnumerator"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="begin"> 시작 위치의 열거자 형식 매개변수를 제공합니다. </param>
		/// <param name="end"> 종료 위치의 열거자 형식 매개변수를 제공합니다. </param>
		/// <param name="getter"> 열거자에서 데이터를 가져오는 방식을 제공합니다. </param>
		/// <param name="instance"> 열거자에서 열거 대상 인스턴스를 보관할 필요가 있을 경우 사용합니다. </param>
		template< class O >
		BasicEnumerator( O begin, O end, std::function<T( object )> getter, object instance = nullptr )
			: begin( begin )
			, end( end )
			, instance( instance )
		{
			increment = []( object arg ) -> object
			{
				auto a = arg.As<O>();
				++a;
				return a;
			};

			get = getter;
		}

		/// <summary> 열거자를 컬렉션의 다음 요소로 이동합니다. </summary>
		/// <exception cref="InvalidOperationException"> 열거자가 만들어진 이후 컬렉션이 수정되었을 경우 발생합니다. </exception>
		bool MoveNext()
		{
			if ( start == false )
			{
				start = true;
				current = begin;
			}
			else
			{
				current = increment( current );
			}

			return !current->Equals( end );
		}

		/// <summary> 컬렉션의 첫 번째 요소 앞의 초기 위치에 열거자를 설정합니다. </summary>
		/// <exception cref="InvalidOperationException"> 열거자가 만들어진 이후 컬렉션이 수정되었을 경우 발생합니다. </exception>
		void Reset()
		{
			start = false;
		}

		/// <summary> 컬렉션에서 열거자의 현재 위치에 있는 요소를 가져옵니다. </summary>
		T Current_get()
		{
			return get( current );
		}
	};
}