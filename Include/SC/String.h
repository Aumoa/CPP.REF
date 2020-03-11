#pragma once

namespace SC
{
	/// <summary> 텍스트를 수정할 수 없는 UTF-16 코드 단위의 시퀸스로 나타냅니다. 이 클래스는 상속될 수 없습니다. </summary>
	class String final : public ValueType, virtual public IEquatable<String>, virtual public IComparable<String>, virtual public IEnumerable<wchar_t>, virtual public ICloneable
	{
		class StringEnumerator;

		std::shared_ptr<std::wstring> ptr;

	public:
		/// <summary> 빈 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		String();

		/// <summary> 멀티바이트 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 멀티바이트 텍스트를 전달합니다. </param>
		String( const std::string& text );

		/// <summary> 멀티바이트 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 멀티바이트 텍스트를 전달합니다. </param>
		String( const std::string_view& text );

		/// <summary> 멀티바이트 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 멀티바이트 텍스트를 전달합니다. </param>
		String( const char* text );

		/// <summary> 유니코드 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 유니코드 텍스트를 전달합니다. </param>
		String( const std::wstring& text );

		/// <summary> 유니코드 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 유니코드 텍스트를 전달합니다. </param>
		String( std::wstring&& text );

		/// <summary> 유니코드 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 유니코드 텍스트를 전달합니다. </param>
		String( const std::wstring_view& text );

		/// <summary> 유니코드 텍스트를 사용하여 <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="text"> 유니코드 텍스트를 전달합니다. </param>
		String( const wchar_t* text );

		/// <summary> <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		String( const String& copy );

		/// <summary> <see cref="String"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		String( String&& mov );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 <see cref="String"/> 개체의 참조를 반환합니다. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( String right ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		int CompareTo( String right ) override;

		/// <summary> (<see cref="IEnumerable"/> 인터페이스에서 구현 됨.) 컬렉션 전체를 반복하는 열거자를 반환합니다. </summary>
		RefPtr<IEnumerator<wchar_t>> GetEnumerator() override;

		/// <summary> (<see cref="ICloneable"/> 인터페이스에서 구현 됨.) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> 현재 개체에서 지정한 문자열을 발견한 인덱스를 보고합니다. </summary>
		/// <param name="value"> 찾을 문자열을 전달합니다. </param>
		/// <param name="startIndex"> 시작 위치를 전달합니다. </param>
		/// <returns> 찾은 문자열의 인덱스를 반환합니다. 찾지 못했을 경우 -1을 반환합니다. </returns>
		int IndexOf( String value, int startIndex = 0 ) const;

		/// <summary> 현재 개체에서 지정한 문자를 발견한 인덱스를 보고합니다. </summary>
		/// <param name="value"> 찾을 문자를 전달합니다. </param>
		/// <param name="startIndex"> 시작 위치를 전달합니다. </param>
		/// <returns> 찾은 문자열의 인덱스를 반환합니다. 찾지 못했을 경우 -1을 반환합니다. </returns>
		int IndexOf( wchar_t value, int startIndex = 0 ) const;

		/// <summary> 현재 개체에서 지정한 위치에서 시작하는 부분 문자열을 생성하여 반환합니다. </summary>
		/// <param name="startIndex"> 시작 위치를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> 시작 위치가 문자열의 길이보다 클 경우 발생합니다. </exception>
		String Substring( int startIndex ) const;

		/// <summary> 현재 개체에서 지정한 위치에서 시작하는 부분 문자열을 생성하여 반환합니다. </summary>
		/// <param name="startIndex"> 시작 위치를 전달합니다. </param>
		/// <param name="length"> 가져올 문자열 길이를 전달합니다. </param>
		/// <exception cref="ArgumentOutOfRangeException"> 시작 위치와 길이의 합이 문자열의 길이보다 클 경우 발생합니다. </exception>
		String Substring( int startIndex, int length ) const;

		/// <summary> (Visual Studio 전용) 현재 텍스트 표현의 소문자 표현을 가져옵니다. </summary>
		vs_property_get( String, Lowercase );

		/// <summary> 현재 텍스트 표현의 소문자 표현을 가져옵니다. </summary>
		String Lowercase_get() const;

		/// <summary> (Visual Studio 전용) 현재 텍스트 표현의 대문자 표현을 가져옵니다. </summary>
		vs_property_get( String, Uppercase );

		/// <summary> 현재 텍스트 표현의 대문자 표현을 가져옵니다. </summary>
		String Uppercase_get() const;

		/// <summary> 현재 개체가 소유하는 <see cref="std::wstring"/> 개체를 가져옵니다. </summary>
		const std::wstring& C_Str() const;

		/// <summary> (Visual Studio 전용) 현재 개체의 문자 수를 가져옵니다. </summary>
		vs_property_get( int, Length );

		/// <summary> 현재 개체의 문자 수를 가져옵니다. </summary>
		int Length_get() const;

		/// <summary> (Visual Studio 전용) 현재 개체의 텍스트 시퀸스를 가져옵니다. </summary>
		vs_property_get( const wchar_t*, Chars );

		/// <summary> 현재 개체의 텍스트 시퀸스를 가져옵니다. </summary>
		const wchar_t* Chars_get() const;

		/// <summary> (Visual Studio 전용) 현재 개체의 텍스트가 빈 텍스트인지 나타내는 값을 가져옵니다. </summary>
		vs_property_get( bool, IsEmpty );

		/// <summary> 현재 개체의 텍스트가 빈 텍스트인지 나타내는 값을 가져옵니다. </summary>
		bool IsEmpty_get() const;

		/// <summary> 지정된 형식에 따라 개체의 값을 문자열로 변환하여 새 문자열 개체를 생성합니다. </summary>
		/// <param name="formatText"> 합성 서식 텍스트입니다. </param>
		/// <param name="args"> 텍스트 표현 개체를 전달합니다. </param>
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