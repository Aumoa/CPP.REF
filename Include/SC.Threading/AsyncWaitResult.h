#pragma once

namespace SC::Threading
{
	/// <summary> �񵿱� ��ü ��� ����� ǥ���մϴ�. </summary>
	enum class AsyncWaitResult
	{
		/// <summary> �ش� ��ü�� ���� �����尡 ����Ǿ����� ��������� �������� �ʾҽ��ϴ�. </summary>
		Abandoned,

		/// <summary> �ش� ��ü�� �۾��� �Ϸ�Ǿ� ��� ����� ��ȯ�Ǿ����ϴ�. </summary>
		Completed,

		/// <summary> �ش� ��ü�� �۾� ��� �ð��� ����Ǿ� ��� ����� ��ȯ�Ǿ����ϴ�. </summary>
		Timeout,

		/// <summary> �ش� ��ü�� ��� ����� �����Ͽ����ϴ�. �������� �ʴ� ��ü�̰ų� ������ ������ �� �ֽ��ϴ�. </summary>
		Failed,
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Threading::AsyncWaitResult value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Abandoned",
		L"Completed",
		L"Timeout",
		L"Failed",
	};

	if ( ( int )value >= 0 && ( int )value <= 3 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Threading.AsyncStatus(" << ( int )value << L")";
	}
}