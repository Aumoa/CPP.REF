#pragma once

namespace SC
{
	/// <summary> ����Ƽ�� ������ ���Ŀ� ���� ���� �Լ��� �����մϴ�. </summary>
	struct IntPtr : public ValueType, virtual public IEquatable<void*>, virtual public IComparable<void*>
	{
		/// <summary> ����Ƽ�� ������ ���� �����մϴ�. </summary>
		void* Value;

		/// <summary> <see cref="IntPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		IntPtr()
			: Value( nullptr )
		{

		}

		/// <summary> <see cref="IntPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		IntPtr( void* ptr )
			: Value( ptr )
		{

		}

		/// <summary> <see cref="IntPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		IntPtr( const IntPtr& copy )
			: Value( copy.Value )
		{

		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override
		{
			std::wostringstream woss;
			if ( Value )
			{
				woss << L"SC.IntPtr(" << std::hex << std::showbase << std::setw( sizeof( void* ) * 2 ) << std::setfill( L'0' ) << Value << L")";
				return woss.str().c_str();
			}
			else
			{
				return L"SC.IntPtr(nullptr)";
			}
		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override
		{
			if ( IntPtr v; obj.Is<IntPtr>( &v ) )
			{
				return Equals( v.Value );
			}
			else
			{
				return obj.Get();
			}
		}

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( void* right ) override
		{
			return Value == right;
		}

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		int CompareTo( void* right ) override
		{
			if ( Value < right ) return -1;
			else if ( Value > right ) return 1;
			else return 0;
		}

		operator void*()
		{
			return Value;
		}

		IntPtr& operator=( const IntPtr& copy )
		{
			Value = copy.Value;
			return *this;
		}
	};
}