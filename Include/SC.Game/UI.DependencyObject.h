#pragma once

namespace SC::Game::UI
{
	/// <summary> ���� ���迡 ���� �Ӽ� �ý��ۿ� �����ϴ� ��� UI ��ü�� ���� ���� �Լ��� �����մϴ�. </summary>
	class DependencyObject : virtual public SC::Object
	{
	public:
		using NameValueMap = std::map<std::size_t, std::wstring>;
		using ClassKeyMap = std::map<std::size_t, NameValueMap>;

	private:
		ClassKeyMap properties;

	protected:
		/// <summary> <see cref="DependencyObject"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		DependencyObject();

	public:
		/// <summary> �� ���� ���� �Ӽ��� �߰��մϴ�. </summary>
		/// <param name="property"> �Ӽ� ��ü�� �����մϴ�. </param>
		void AddDependencyProperty( DependencyProperty property );

		/// <summary> �� ���� ���� �Ӽ��� �߰��մϴ�. </summary>
		/// <param name="property"> �Ľ� ������ �Ӽ� �ؽ�Ʈ�� �����մϴ�. </param>
		void AddDependencyProperty( String property )
		{
			return AddDependencyProperty( DependencyProperty( property ) );
		}

		/// <summary> ���� ���� �Ӽ��� �����մϴ�. </summary>
		/// <param name="className"> ������ �Ӽ��� ���� Ŭ���� �̸��� �����մϴ�. </param>
		/// <param name="propertyName"> ������ �Ӽ��� �̸��� �����մϴ�. </param>
		void RemoveDependencyProperty( std::size_t className, std::size_t propertyName );

		/// <summary> ���� ���� �Ӽ��� �����մϴ�. </summary>
		/// <param name="className"> ������ �Ӽ��� ���� Ŭ���� �̸��� �����մϴ�. </param>
		void RemoveDependencyProperty( std::size_t className );

		/// <summary> Ŭ���� �̸��� ���Ե� �Ӽ� �� ����� �����ɴϴ�. </summary>
		/// <param name="className"> Ŭ���� �̸� �ؽð��� �����մϴ�. </param>
		/// <param name="pNameValueMap"> ���� ���޹��� �ּҸ� �����մϴ�. </param>
		bool TryGetClassProperties( std::size_t className, NameValueMap* pNameValueMap );
	};
}