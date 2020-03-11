#pragma once

namespace SC::Game::UI
{
	/// <summary> 의존 관계에 의한 속성 시스템에 참여하는 모든 UI 개체에 대한 공통 함수를 제공합니다. </summary>
	class DependencyObject : virtual public SC::Object
	{
	public:
		using NameValueMap = std::map<std::size_t, std::wstring>;
		using ClassKeyMap = std::map<std::size_t, NameValueMap>;

	private:
		ClassKeyMap properties;

	protected:
		/// <summary> <see cref="DependencyObject"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		DependencyObject();

	public:
		/// <summary> 새 의존 관계 속성을 추가합니다. </summary>
		/// <param name="property"> 속성 개체를 전달합니다. </param>
		void AddDependencyProperty( DependencyProperty property );

		/// <summary> 새 의존 관계 속성을 추가합니다. </summary>
		/// <param name="property"> 파싱 가능한 속성 텍스트를 전달합니다. </param>
		void AddDependencyProperty( String property )
		{
			return AddDependencyProperty( DependencyProperty( property ) );
		}

		/// <summary> 의존 관계 속성을 제거합니다. </summary>
		/// <param name="className"> 제거할 속성의 소유 클래스 이름을 전달합니다. </param>
		/// <param name="propertyName"> 제거할 속성의 이름을 전달합니다. </param>
		void RemoveDependencyProperty( std::size_t className, std::size_t propertyName );

		/// <summary> 의존 관계 속성을 제거합니다. </summary>
		/// <param name="className"> 제거할 속성의 소유 클래스 이름을 전달합니다. </param>
		void RemoveDependencyProperty( std::size_t className );

		/// <summary> 클래스 이름에 포함된 속성 값 목록을 가져옵니다. </summary>
		/// <param name="className"> 클래스 이름 해시값을 전달합니다. </param>
		/// <param name="pNameValueMap"> 값을 전달받을 주소를 전달합니다. </param>
		bool TryGetClassProperties( std::size_t className, NameValueMap* pNameValueMap );
	};
}