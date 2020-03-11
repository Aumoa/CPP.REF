#pragma once

namespace SC::Threading
{
	/// <summary> �۾� ����, �۾� �׸� �Խ�, �񵿱� I/O ó��, �ٸ� ������ ��� ��ٸ��� �� Ÿ�̸� ó�� � ����� �� �ִ� ������ Ǯ �Լ��� �����մϴ�. </summary>
	class ThreadPool : virtual public Object
	{
	public:
		/// <summary> ������ ���� �񵿱� �Լ��� ������ ť�� ����ŵ�ϴ�. </summary>
		/// <param name="asyncTask"> ������ �񵿱� �Լ��� �����մϴ�. </param>
		/// <param name="args"> �񵿱� �Լ��� ������ �Ű������� �����մϴ�. </param>
		/// <remarks> �� �Լ��� ���� ������ ���� ��ɿ� ����ȭ�Ǿ� �ֽ��ϴ�. ���� ���� �ð� �۾��ϴ� �����带 ������ ��� CreateTask �Լ��� ����ϼ���. </remarks>
		static RefPtr<IAsyncOperation<object>> QueueUserWorkItem( AsyncTaskDelegate<object> asyncTask, object args );

		/// <summary> ������ ���� �񵿱� �Լ��� ������ ť�� ����ŵ�ϴ�. </summary>
		/// <param name="asyncTask"> ������ �񵿱� �Լ��� �����մϴ�. </param>
		/// <param name="args"> �񵿱� �Լ��� ������ �Ű������� �����մϴ�. </param>
		/// <remarks> �� �Լ��� ���� ���� �ð� ���� ��ɿ� ����ȭ�Ǿ� �ֽ��ϴ�. ������ ���� ����� ����ϴ� �����带 ������ ��� QueueUserWorkItem �Լ��� ����ϼ���. </remarks>
		static RefPtr<IAsyncOperation<object>> CreateTask( AsyncTaskDelegate<object> asyncTask, object args );
	};
}