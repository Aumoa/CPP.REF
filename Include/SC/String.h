#pragma once

namespace SC
{
	/// <summary> �ؽ�Ʈ�� ������ �� ���� UTF-16 �ڵ� ������ �������� ��Ÿ���ϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class String final : public ValueType, virtual public IEquatable<String>, virtual public IComparable<String>, virtual public IEnumerable<wchar_t>, virtual public ICloneable
	{
		class StringEnumerator;

		std::shared_ptr<std::wstring> ptr;

	public:
		/// <summary> �� �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		String();

		/// <summary> ��Ƽ����Ʈ �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> ��Ƽ����Ʈ �ؽ�Ʈ�� �����մϴ�. </param>
		String( const std::string& text );

		/// <summary> ��Ƽ����Ʈ �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> ��Ƽ����Ʈ �ؽ�Ʈ�� �����մϴ�. </param>
		String( const std::string_view& text );

		/// <summary> ��Ƽ����Ʈ �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> ��Ƽ����Ʈ �ؽ�Ʈ�� �����մϴ�. </param>
		String( const char* text );

		/// <summary> �����ڵ� �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> �����ڵ� �ؽ�Ʈ�� �����մϴ�. </param>
		String( const std::wstring& text );

		/// <summary> �����ڵ� �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> �����ڵ� �ؽ�Ʈ�� �����մϴ�. </param>
		String( std::wstring&& text );

		/// <summary> �����ڵ� �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> �����ڵ� �ؽ�Ʈ�� �����մϴ�. </param>
		String( const std::wstring_view& text );

		/// <summary> �����ڵ� �ؽ�Ʈ�� ����Ͽ� <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="text"> �����ڵ� �ؽ�Ʈ�� �����մϴ�. </param>
		String( const wchar_t* text );

		/// <summary> <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		String( const String& copy );

		/// <summary> <see cref="String"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		String( String&& mov );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� <see cref="String"/> ��ü�� ������ ��ȯ�մϴ�. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( String right ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		int CompareTo( String right ) override;

		/// <summary> (<see cref="IEnumerable"/> �������̽����� ���� ��.) �÷��� ��ü�� �ݺ��ϴ� �����ڸ� ��ȯ�մϴ�. </summary>
		RefPtr<IEnumerator<wchar_t>> GetEnumerator() override;

		/// <summary> (<see cref="ICloneable"/> �������̽����� ���� ��.) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> ���� ��ü���� ������ ���ڿ��� �߰��� �ε����� �����մϴ�. </summary>
		/// <param name="value"> ã�� ���ڿ��� �����մϴ�. </param>
		/// <param name="startIndex"> ���� ��ġ�� �����մϴ�. </param>
		/// <returns> ã�� ���ڿ��� �ε����� ��ȯ�մϴ�. ã�� ������ ��� -1�� ��ȯ�մϴ�. </returns>
		int IndexOf( String value, int startIndex = 0 ) const;

		/// <summary> ���� ��ü���� ������ ���ڸ� �߰��� �ε����� �����մϴ�. </summary>
		/// <param name="value"> ã�� ���ڸ� �����մϴ�. </param>
		/// <param name="startIndex"> ���� ��ġ�� �����մϴ�. </param>
		/// <returns> ã�� ���ڿ��� �ε����� ��ȯ�մϴ�. ã�� ������ ��� -1�� ��ȯ�մϴ�. </returns>
		int IndexOf( wchar_t value, int startIndex = 0 ) const;

		/// <summary> ���� ��ü���� ������ ��ġ���� �����ϴ� �κ� ���ڿ��� �����Ͽ� ��ȯ�մϴ�. </summary>
		/// <param name="startIndex"> ���� ��ġ�� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> ���� ��ġ�� ���ڿ��� ���̺��� Ŭ ��� �߻��մϴ�. </exception>
		String Substring( int startIndex ) const;

		/// <summary> ���� ��ü���� ������ ��ġ���� �����ϴ� �κ� ���ڿ��� �����Ͽ� ��ȯ�մϴ�. </summary>
		/// <param name="startIndex"> ���� ��ġ�� �����մϴ�. </param>
		/// <param name="length"> ������ ���ڿ� ���̸� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> ���� ��ġ�� ������ ���� ���ڿ��� ���̺��� Ŭ ��� �߻��մϴ�. </exception>
		String Substring( int startIndex, int length ) const;

		/// <summary> (Visual Studio ����) ���� �ؽ�Ʈ ǥ���� �ҹ��� ǥ���� �����ɴϴ�. </summary>
		vs_property_get( String, Lowercase );

		/// <summary> ���� �ؽ�Ʈ ǥ���� �ҹ��� ǥ���� �����ɴϴ�. </summary>
		String Lowercase_get() const;

		/// <summary> (Visual Studio ����) ���� �ؽ�Ʈ ǥ���� �빮�� ǥ���� �����ɴϴ�. </summary>
		vs_property_get( String, Uppercase );

		/// <summary> ���� �ؽ�Ʈ ǥ���� �빮�� ǥ���� �����ɴϴ�. </summary>
		String Uppercase_get() const;

		/// <summary> ���� ��ü�� �����ϴ� <see cref="std::wstring"/> ��ü�� �����ɴϴ�. </summary>
		const std::wstring& C_Str() const;

		/// <summary> (Visual Studio ����) ���� ��ü�� ���� ���� �����ɴϴ�. </summary>
		vs_property_get( int, Length );

		/// <summary> ���� ��ü�� ���� ���� �����ɴϴ�. </summary>
		int Length_get() const;

		/// <summary> (Visual Studio ����) ���� ��ü�� �ؽ�Ʈ �������� �����ɴϴ�. </summary>
		vs_property_get( const wchar_t*, Chars );

		/// <summary> ���� ��ü�� �ؽ�Ʈ �������� �����ɴϴ�. </summary>
		const wchar_t* Chars_get() const;

		/// <summary> (Visual Studio ����) ���� ��ü�� �ؽ�Ʈ�� �� �ؽ�Ʈ���� ��Ÿ���� ���� �����ɴϴ�. </summary>
		vs_property_get( bool, IsEmpty );

		/// <summary> ���� ��ü�� �ؽ�Ʈ�� �� �ؽ�Ʈ���� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsEmpty_get() const;

		/// <summary> ������ ���Ŀ� ���� ��ü�� ���� ���ڿ��� ��ȯ�Ͽ� �� ���ڿ� ��ü�� �����մϴ�. </summary>
		/// <param name="formatText"> �ռ� ���� �ؽ�Ʈ�Դϴ�. </param>
		/// <param name="args"> �ؽ�Ʈ ǥ�� ��ü�� �����մϴ�. </param>
		template< class... _Args >
		static inline String Format( String formatText, _Args&&... args );

		String operator+( String right ) const;
		String operator-( int count ) const;

		String& operator=( String right );
		String& operator+=( String right );

		bool operator< ( String right ) const;
		bool operator<=( String right ) const;
		bool operator> ( String right ) const;
		bool operator>=( String right ) const;
		bool operator==( String right ) const;
		bool operator!=( String right ) const;

		bool operator< ( std::wstring_view right ) const;
		bool operator<=( std::wstring_view right ) const;
		bool operator> ( std::wstring_view right ) const;
		bool operator>=( std::wstring_view right ) const;
		bool operator==( std::wstring_view right ) const;
		bool operator!=( std::wstring_view right ) const;

	private:
		template< class... _Args >
		static inline void Format_Helper( std::vector<object>& vec, object arg0, _Args&&... args );
		static inline void Format_Helper( std::vector<object>& vec, object arg0 );
		static inline void Format_Helper( std::vector<object>& vec );

		static String Format( String formatText, std::vector<object> args );
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::String right )
{
	return wout << right.Chars_get();
}