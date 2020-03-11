#pragma once

namespace SC::Diagnostics
{
	/// <summary> <see cref="StepTimer"/> Ŭ�������� �������ϴ� �븮�� ������ �����մϴ�. </summary>
	struct StepTimerCallbackDelegate : public Delegate
	{
	private:
		std::function<void( RefPtr<StepTimer> sender )> callback;

	public:
		/// <summary> <see cref="StepTimerCallbackDelegate"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		StepTimerCallbackDelegate() = default;

		/// <summary> <see cref="StepTimerCallbackDelegate"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="callback"> �Լ��� �����մϴ�. </param>
		StepTimerCallbackDelegate( std::function<void( RefPtr<StepTimer> sender )> callback );

		/// <summary> <see cref="StepTimerCallbackDelegate"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="caller"> �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
		/// <param name="callback"> �Լ��� �����մϴ�. </param>
		template< class T >
		inline StepTimerCallbackDelegate( T* caller, void( T::*callback )( RefPtr<StepTimer> sender ) );

		/// <summary> <see cref="StepTimerCallbackDelegate"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="caller"> �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
		/// <param name="callback"> �Լ��� �����մϴ�. </param>
		template< class T >
		inline StepTimerCallbackDelegate( RefPtr<T> caller, void( T::*callback )( RefPtr<StepTimer> sender ) );

		/// <summary> <see cref="StepTimerCallbackDelegate"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		StepTimerCallbackDelegate( const StepTimerCallbackDelegate& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override;

		/// <summary> (Visual Studio ����) ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
		vs_property( bool, IsValid );

		/// <summary> ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
		bool IsValid_get();

		/// <summary> �Լ��� ȣ���մϴ�. </summary>
		/// <param name="sender"> ȣ���� ��ü�� �����մϴ�. </param>
		void Invoke( RefPtr<StepTimer> sender );

		void operator()( RefPtr<StepTimer> sender );
	};
}