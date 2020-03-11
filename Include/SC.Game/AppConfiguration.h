#pragma once

namespace SC::Game
{
	/// <summary> ���� �� ȯ�� ������ ǥ���մϴ�. </summary>
	class AppConfiguration : public ValueType
	{
		friend class Application;

	private:
		String appName;
		bool fullscreenMode = false;
		double scaleFactor = 1.0;
		int physicsFPS = 60;
		bool disableUI = false;
		String deviceName;
		bool verticalSync = false;

	public:
		/// <summary> <see cref="AppConfiguration"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		AppConfiguration();

		/// <summary> <see cref="AppConfiguration"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		AppConfiguration( const AppConfiguration& copy );

		/// <summary> (<see cref="SC::Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override;

		/// <summary> (Visual Studio ����) ���� �̸��� �����ϰų� �����ɴϴ�. </summary>
		vs_property( String, AppName );

		/// <summary> ���� �̸��� �����ɴϴ�. </summary>
		String AppName_get();

		/// <summary> ���� �̸��� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void AppName_set( String value );

		/// <summary> (Visual Studio ����) ���� ��ü ȭ�� ��带 ����ϴ��� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. �÷����� ��ü ȭ���� �������� �ʴ� ��� �׻� true�� ��ȯ�ϸ�, ���� ���õ˴ϴ�. </summary>
		vs_property( bool, FullscreenMode );

		/// <summary> ���� ��ü ȭ�� ��带 ����ϴ��� ��Ÿ���� ���� �����ɴϴ�. �÷����� ��ü ȭ���� �������� �ʴ� ��� �׻� true�� ��ȯ�մϴ�. </summary>
		bool FullscreenMode_get();

		/// <summary> ���� ��ü ȭ�� ��带 ����ϴ��� ��Ÿ���� ���� �����մϴ�. �÷����� ��ü ȭ���� �������� �ʴ� ��� ���� ���õ˴ϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void FullscreenMode_set( bool value );

		/// <summary> (Visual Studio ����) UI Ȯ�� ������ �����ϰų� �����ɴϴ�. 1 ~ 5 ������ ���� �����մϴ�. </summary>
		vs_property( double, ScaleFactor );

		/// <summary> UI Ȯ�� ������ �����ɴϴ�. </summary>
		double ScaleFactor_get();

		/// <summary> UI Ȯ�� ������ �����մϴ�. 1 ~ 5 ������ ���� �����մϴ�. </summary>
		void ScaleFactor_set( double value );

		/// <summary> (Visual Studio ����) �ʴ� ���� ���� Ƚ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, PhysicsUpdatePerSeconds );

		/// <summary> �ʴ� ���� ���� Ƚ���� �����ɴϴ�. </summary>
		int PhysicsUpdatePerSeconds_get();

		/// <summary> �ʴ� ���� ���� Ƚ���� �����մϴ�. </summary>
		void PhysicsUpdatePerSeconds_set( int value );

		/// <summary> (Visual Studio ����) ���� UI ������ ��� Ȱ��ȭ ���¸� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, DisableUI );

		/// <summary> ���� UI ������ ��� Ȱ��ȭ ���¸� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool DisableUI_get();

		/// <summary> ���� UI ������ ��� Ȱ��ȭ ���¸� ��Ÿ���� ���� �����մϴ�. </summary>
		void DisableUI_set( bool value );

		/// <summary> (Visual Studio ����) ���� �׷��� ��ġ�� ����ϴ� ���� ����� �̸��� �����ɴϴ�. </summary>
		vs_property_get( String, AdapterName );

		/// <summary> ���� �׷��� ��ġ�� ����ϴ� ���� ����� �̸��� �����ɴϴ�. </summary>
		String AdapterName_get();

		/// <summary> (Visual Studio ����) ���� ���� ����ȭ ��� Ȱ��ȭ ���¸� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, VSync );

		/// <summary> ���� ���� ����ȭ ��� Ȱ��ȭ ���¸� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool VSync_get();

		/// <summary> ���� ���� ����ȭ ��� Ȱ��ȭ ���¸� ��Ÿ���� ���� �����մϴ�. </summary>
		void VSync_set( bool value );

		AppConfiguration& operator=( const AppConfiguration& copy );
	};
}