#pragma once

namespace SC
{
	/// <summary> ���� ���谡 �ʿ��� �Ӽ� ������ �����մϴ�. </summary>
	struct DependencyProperty : public ValueType
	{
		/// <summary> ���� ��� Ŭ���� �̸��� �ؽ� ���� �����մϴ�. </summary>
		std::size_t ClassName;

		/// <summary> ���� ��� Ŭ������ �Ӽ� �̸� �ؽ� ���� �����մϴ�. </summary>
		std::size_t PropertyName;

		/// <summary> ���� ��� Ŭ������ �Ӽ� ���� �����մϴ�. </summary>
		String PropertyValue;

		/// <summary> <see cref="DependencyProperty"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		DependencyProperty()
		{

		}

		/// <summary> <see cref="DependencyProperty"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="parseText"> ���� �ؽ�Ʈ�� "ClassName.PropertyName=Value" �������� �����մϴ�. </param>
		DependencyProperty( String parseText )
		{
			std::hash<std::wstring> hash;
			String className;
			String propertyName;
			TryParse( parseText, className, propertyName, PropertyValue );
			ClassName = hash( className.C_Str() );
			PropertyName = hash( propertyName.C_Str() );
		}

		/// <summary> <see cref="DependencyProperty"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		DependencyProperty( String className, String propertyName, String propertyValue )
			: PropertyValue( propertyValue )
		{
			std::hash<std::wstring> hash;
			ClassName = hash( className.C_Str() );
			PropertyName = hash( propertyName.C_Str() );
		}

		/// <summary> <see cref="DependencyProperty"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		DependencyProperty( std::size_t ClassName, std::size_t PropertyName, String propertyValue )
			: ClassName( ClassName )
			, PropertyName( PropertyName )
			, PropertyValue( propertyValue )
		{

		}

		/// <summary> <see cref="DependencyProperty"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
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