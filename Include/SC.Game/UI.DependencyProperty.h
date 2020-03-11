#pragma once

namespace SC
{
	/// <summary> 의존 관계가 필요한 속성 설정을 수행합니다. </summary>
	struct DependencyProperty : public ValueType
	{
		/// <summary> 의존 대상 클래스 이름의 해시 값을 설정합니다. </summary>
		std::size_t ClassName;

		/// <summary> 의존 대상 클래스의 속성 이름 해시 값을 설정합니다. </summary>
		std::size_t PropertyName;

		/// <summary> 의존 대상 클래스의 속성 값을 설정합니다. </summary>
		String PropertyValue;

		/// <summary> <see cref="DependencyProperty"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		DependencyProperty()
		{

		}

		/// <summary> <see cref="DependencyProperty"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="parseText"> 구문 텍스트를 "ClassName.PropertyName=Value" 형식으로 제공합니다. </param>
		DependencyProperty( String parseText )
		{
			std::hash<std::wstring> hash;
			String className;
			String propertyName;
			TryParse( parseText, className, propertyName, PropertyValue );
			ClassName = hash( className.C_Str() );
			PropertyName = hash( propertyName.C_Str() );
		}

		/// <summary> <see cref="DependencyProperty"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		DependencyProperty( String className, String propertyName, String propertyValue )
			: PropertyValue( propertyValue )
		{
			std::hash<std::wstring> hash;
			ClassName = hash( className.C_Str() );
			PropertyName = hash( propertyName.C_Str() );
		}

		/// <summary> <see cref="DependencyProperty"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		DependencyProperty( std::size_t ClassName, std::size_t PropertyName, String propertyValue )
			: ClassName( ClassName )
			, PropertyName( PropertyName )
			, PropertyValue( propertyValue )
		{

		}

		/// <summary> <see cref="DependencyProperty"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		DependencyProperty( const DependencyProperty& copy )
			: ClassName( copy.ClassName )
			, PropertyName( copy.PropertyName )
			, PropertyValue( copy.PropertyValue )
		{

		}

	private:
		static bool TryParse( String parseText, String& className, String& propertyName, String& propertyValue )
		{
			int classDot = parseText.IndexOf( L'.' );
			if ( classDot == -1 ) return false;

			int nameEqual = parseText.IndexOf( L'=' );
			if ( nameEqual == -1 ) return false;

			className = parseText.Substring( 0, classDot );
			propertyName = parseText.Substring( classDot + 1, nameEqual - classDot - 1 );
			propertyValue = parseText.Substring( nameEqual + 1 );

			return true;
		}
	};
}