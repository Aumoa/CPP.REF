#pragma once

namespace SC::Game
{
	/// <summary> �ۿ��� ó������ ���� �񵿱� �Ϸ� ó���� �Ǵ� �̺�Ʈ ó���⿡�� �߻��� ���� ��ü�� �����ϴ� �̺�Ʈ �������Դϴ�. </summary>
	class UnhandledErrorDetectedEventArgs : public EventArgs
	{
		Exception* e = nullptr;
		bool isCritical = true;

	public:
		/// <summary> <see cref="UnhandledErrorDetectedEventArgs"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="e"> ���� ��ü�� �����մϴ�. </param>
		UnhandledErrorDetectedEventArgs( Exception* e );
		~UnhandledErrorDetectedEventArgs() override;

		/// <summary> (<see cref="EventArgs"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override;

		/// <summary> (Visual Studio ����) �ۿ��� ó������ ���� ���� ��ü�� �����ɴϴ�. </summary>
		vs_property_get( SC::Exception*, Exception );

		/// <summary> �ۿ��� ó������ ���� ���� ��ü�� �����ɴϴ�. </summary>
		SC::Exception* Exception_get();

		/// <summary> (Visual Studio ����) �� ���ܸ� �ɰ��� ���ܷ� �����ϴ� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsCritical );

		/// <summary> �� ���ܸ� �ɰ��� ���ܷ� �����ϴ� ���� �����ɴϴ�. </summary>
		bool IsCritical_get();

		/// <summary> �� ���ܸ� �ɰ��� ���ܷ� �����ϴ� ���� �����մϴ�. </summary>
		void IsCritical_set( bool value );
	};
}