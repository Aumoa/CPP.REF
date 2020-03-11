#pragma once

namespace SC::Threading
{
	/// <summary> �񵿱� ��ü���� �ý��ۿ��� ��� ����� ������ �� �ִ� ��ü�� ���� ���� �Լ��� �����մϴ�. </summary>
	class AsyncWait : virtual public Object
	{
		void* native_handle = nullptr;

	protected:
		AsyncWait( void* handle );

		virtual bool CustomRelease( void* native_handle );

	public:
		~AsyncWait() override;

		/// <summary> ���� ��ü�� ���� ��� ����� �����մϴ�. </summary>
		/// <param name="milliseconds"> ��� ����� Timeout �ð��� �����մϴ�. 0xFFFFFFFF ���� ���޵� ��� ���� ����մϴ�. </param>
		AsyncWaitResult WaitForSingleObject( uint32 milliseconds = 0xFFFFFFFF ) const;

		/// <summary> (Visual Studio ����) ���� ��� ��ü�� ����� �ڵ��� �����ɴϴ�. </summary>
		vs_property_get( void*, Handle );

		/// <summary> ���� ��� ��ü�� ����� �ڵ��� �����ɴϴ�. </summary>
		void* Handle_get() const;

		/// <summary> �����ϴ� ��� ��ü�� ���� ��� ����� �����մϴ�. </summary>
		/// <param name="numAsyncWaitObjects"> ����� ��ü ������ �����մϴ�. </param>
		/// <param name="asyncWaitObjects"> ����� ��ü ����� �����մϴ�. </param>
		/// <param name="milliseconds"> ��� ����� Timeout �ð��� �����մϴ�. 0xFFFFFFFF ���� ���޵� ��� ���� ����մϴ�. </param>
		/// <remarks> ��� ��ü�� ���� ���°� �Ǹ� <see cref="AsyncWaitResult::Completed"/> ���¸� ��ȯ�մϴ�. </remarks>
		//static AsyncWaitResult WaitForAllObjects( int numAsyncWaitObjects, const AsyncWait asyncWaitObjects[], uint32 milliseconds );
	};
}