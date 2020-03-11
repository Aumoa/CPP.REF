#pragma once

namespace SC
{
	/// <summary> SC Ŭ���� ���� ���� ������ ��Ʈ�Դϴ�. �Ļ� Ŭ������ ���� ���� �⺻ ���񽺸� �����մϴ�. </summary>
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
		/// <summary> <see cref="Object"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Object();
		virtual ~Object();

		/// <summary> ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		virtual String ToString();

		/// <summary> ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		virtual bool Equals( object obj );

		/// <summary> (Visual Studio ����) ��ü�� ���� ���� ���¸� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsLocked );

		/// <summary> ��ü�� ���� ���� ���¸� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsLocked_get();

		/// <summary> ��ü�� ���� ���� ���¸� ��Ÿ���� ���� �����մϴ�. </summary>
		void IsLocked_set( bool value );

		/// <summary> ������ ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		/// <param name="obj"> ��ü�� �����մϴ�. </param>
		static String ToString( object obj );

		/// <summary> ������ ��ü �ν��Ͻ��� �������� Ȯ���մϴ�. </summary>
		/// <param name="objA"> ���� ù ��° ��ü�� �����մϴ�. ��ü�� null�� �ƴ� ��� ��ü�� ������ �Լ� Equals�� ȣ��˴ϴ�. </param>
		/// <param name="objB"> ���� �� ��° ��ü�� �����մϴ�. </param>
		static bool Equals( object objA, object objB );

		/// <summary> ������ <see cref="Object"/> �ν��Ͻ��� �������� ���θ� Ȯ���մϴ�. </summary>
		/// <param name="objA"> ���� ù ��° ��ü�� �����մϴ�. ��ü�� null�� �ƴ� ��� ��ü�� ������ �Լ� Equals�� ȣ��˴ϴ�. </param>
		/// <param name="objB"> ���� �� ��° ��ü�� �����մϴ�. </param>
		static bool ReferenceEquals( object objA, object objB );

		/// <summary> �� Ŭ������ ��ӹ޴� ��� Ŭ������ �ν��Ͻ� ������ �����ɴϴ�. </summary>
		static int GetInstanceCount();

		bool operator==( object right );
		bool operator!=( object right );
	};
}