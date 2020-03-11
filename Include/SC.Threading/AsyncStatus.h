#pragma once

namespace SC::Threading
{
	/// <summary> �񵿱� �۾��� ���¸� ǥ���մϴ�. </summary>
	enum class AsyncStatus
	{
		/// <summary> �۾� ���°� �Խõ��� �ʾҽ��ϴ�. �Ϲ������� �� ��Ȳ�� ������ �����մϴ�. </summary>
		Unspecified = -1,

		/// <summary> �۾��� ���۵Ǿ����ϴ�. </summary>
		Started = 0,

		/// <summary> �۾��� �Ϸ�Ǿ����ϴ�. </summary>
		Completed = 1,

		/// <summary> �۾��� ��ҵǾ����ϴ�. </summary>
		Canceled = 2,
		
		/// <summary> �۾��� �����ϴ� �� ������ �߻��Ͽ����ϴ�. </summary>
		Error = 3,

		/// <summary> �۾��� ������Դϴ�. </summary>
		Pending = 4,
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Threading::AsyncStatus value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Unspecified",
		L"Started",
		L"Completed",
		L"Canceled",
		L"Error",
		L"Pending",
	};

	if ( ( int )value >= -1 && ( int )value <= 4 )
	{
		return wout << context[( int )value + 1];
	}
	else
	{
		return wout << L"SC.Threading.AsyncStatus(" << ( int )value << L")";
	}
}