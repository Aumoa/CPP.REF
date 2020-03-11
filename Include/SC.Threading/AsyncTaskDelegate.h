#pragma once

namespace SC::Threading
{
	/// <summary> ������ Ǯ�� �����ϴ� �񵿱� �۾� �ݹ� �Լ��� ��Ÿ���ϴ�. </summary>
	template< class T >
	using AsyncTaskDelegate = std::function<T( object args )>;

	/// <summary> ü������ ���� �� �۾��� ���� �Ű� ������ ����ϴ� �񵿱� �۾� �ݹ� �Լ��� ��Ÿ���ϴ�. </summary>
	template< class T >
	using AsyncTaskChainDelegate = std::function<T( T results, object args )>;
}