#pragma once

namespace SC::Threading
{
	/// <summary> �񵿱� �۾��� ���� ���� ������ ǥ���մϴ�. </summary>
	struct IAsyncInfo : virtual public Object
	{
		/// <summary> ���� �񵿱� �۾��� ����մϴ�. </summary>
		virtual void Cancel() = 0;

		/// <summary> ���� �񵿱� �۾��� �����մϴ�. </summary>
		virtual void Close() = 0;

		/// <summary> (Visual Studio ����) �񵿱� �۾��� ���� ID�� �����ɴϴ�. </summary>
		vs_property_get( int64, Id );

		/// <summary> �񵿱� �۾��� ���� ID�� �����ɴϴ�. </summary>
		virtual int64 Id_get() = 0;

		/// <summary> (Visual Studio ����) �񵿱� �۾����� �߻��� ���� ������ �����ɴϴ�. </summary>
		vs_property_get( RefPtr<Exception>, Error );

		/// <summary> �񵿱� �۾����� �߻��� ���� ������ �����ɴϴ�. </summary>
		virtual RefPtr<Exception> Error_get() = 0;

		/// <summary> (Visual Studio ����) ���� �񵿱� �۾��� ���¸� ��Ÿ���� ���� �����ɴϴ�. </summary>
		vs_property_get( AsyncStatus, Status );

		/// <summary> ���� �񵿱� �۾��� ���¸� ��Ÿ���� ���� �����ɴϴ�. </summary>
		virtual AsyncStatus Status_get() = 0;
	};
}