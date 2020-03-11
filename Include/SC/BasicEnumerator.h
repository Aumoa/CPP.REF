#pragma once

namespace SC
{
	/// <summary> ++ ������ �� == �����ڸ� �����ϴ� �׸� ���� ���� ������ ������ �����մϴ�. </summary>
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
		/// <summary> <see cref="BasicEnumerator"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="begin"> ���� ��ġ�� ������ ���� �Ű������� �����մϴ�. </param>
		/// <param name="end"> ���� ��ġ�� ������ ���� �Ű������� �����մϴ�. </param>
		/// <param name="getter"> �����ڿ��� �����͸� �������� ����� �����մϴ�. </param>
		/// <param name="instance"> �����ڿ��� ���� ��� �ν��Ͻ��� ������ �ʿ䰡 ���� ��� ����մϴ�. </param>
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

		/// <summary> �����ڸ� �÷����� ���� ��ҷ� �̵��մϴ�. </summary>
		/// <exception cref="InvalidOperationException"> �����ڰ� ������� ���� �÷����� �����Ǿ��� ��� �߻��մϴ�. </exception>
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

		/// <summary> �÷����� ù ��° ��� ���� �ʱ� ��ġ�� �����ڸ� �����մϴ�. </summary>
		/// <exception cref="InvalidOperationException"> �����ڰ� ������� ���� �÷����� �����Ǿ��� ��� �߻��մϴ�. </exception>
		void Reset()
		{
			start = false;
		}

		/// <summary> �÷��ǿ��� �������� ���� ��ġ�� �ִ� ��Ҹ� �����ɴϴ�. </summary>
		T Current_get()
		{
			return get( current );
		}
	};
}