#pragma once

namespace SC::Game::UI
{
	/// <summary> EventDispatcher Ŭ������ �����ϴ� �̺�Ʈ �޽����� ���� �Ű������� ǥ���մϴ�. </summary>
	class DispatcherEventArgs : public EventArgs
	{
		DispatcherEventType type;
		object args;
		bool isCompleted = false;

	public:
		/// <summary> <see cref="DispatcherEventArgs"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="type"> �̺�Ʈ ������ �����մϴ�. </param>
		/// <param name="args"> �̺�Ʈ �Ű������� �����մϴ�. </param>
		DispatcherEventArgs( DispatcherEventType type, object args );

		/// <summary> (Visual Studio ����) �̺�Ʈ ������ �����ɴϴ�. </summary>
		vs_property_get( DispatcherEventType, Type );

		/// <summary> �̺�Ʈ ������ �����ɴϴ�. </summary>
		DispatcherEventType Type_get();

		/// <summary> (Visual Studio ����) �̺�Ʈ �Ű������� �����ɴϴ�. </summary>
		vs_property_get( object, Argument );

		/// <summary> �̺�Ʈ �Ű������� �����ɴϴ�. </summary>
		object Argument_get();

		/// <summary> (Visual Studio ����) �� �̺�Ʈ�� �̹� ó���Ǿ����� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsCompleted );

		/// <summary> �� �̺�Ʈ�� �̹� ó���Ǿ����� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsCompleted_get();

		/// <summary> �� �̺�Ʈ�� �̹� ó���Ǿ����� ��Ÿ���� ���� �����մϴ�. </summary>
		void IsCompleted_set( bool value );
	};
}