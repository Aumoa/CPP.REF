#pragma once

namespace SC::Drawing
{
	/// <summary> 4개의 부동 소수점 형식을 사용하여 색상 정보를 표현합니다. </summary>
	struct Color : public ValueType, virtual public IComparable<Color>, virtual public IEquatable<Color>
	{
		/// <summary> 빨간색 성분 비율을 설정하거나 가져옵니다. </summary>
		float R;

		/// <summary> 초록색 성분 비율을 설정하거나 가져옵니다. </summary>
		float G;

		/// <summary> 파란색 성분 비율을 설정하거나 가져옵니다. </summary>
		float B;

		/// <summary> 알파 성분 비율을 설정하거나 가져옵니다. </summary>
		float A;

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Color();

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Color( const Color& copy );

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="rgb"> RGB 값을 추출할 색상 개체를 전달합니다. </param>
		/// <param name="alphaOverride"> 덮어쓸 새로운 알파 값을 전달합니다. </param>
		Color( const Color& rgb, float alphaOverride );

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Color( float r, float g, float b, float a = 1.0f );

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Color( int r, int g, int b, int a = 255 );

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="rgb"> RGB 색상 코드를 전달합니다. </param>
		/// <param name="a"> 알파값을 전달합니다. </param>
		Color( uint32 rgb, int a );

		/// <summary> <see cref="Color"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="rgb"> RGB 색상 코드를 전달합니다. </param>
		/// <param name="a"> 알파값을 전달합니다. </param>
		Color( uint32 rgb, float a = 1.0f );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 색상의 #ARGB 텍스트 표현을 가져옵니다. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( Color right ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		int CompareTo( Color right ) override;

		/// <summary> 두 색을 알파 블렌드 함수로 섞습니다. </summary>
		Color AlphaBlend( const Color& right ) const;

		/// <summary> 빨간색 성분 비율을 정수 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( int, IntR );

		/// <summary> 빨간색 성분 비율을 정수 형식으로 가져옵니다. </summary>
		int IntR_get() const;

		/// <summary> 빨간색 성분 비율을 정수 형식으로 설정합니다. </summary>
		void IntR_set( int value );

		/// <summary> 초록색 성분 비율을 정수 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( int, IntG );

		/// <summary> 초록색 성분 비율을 정수 형식으로 가져옵니다. </summary>
		int IntG_get() const;

		/// <summary> 초록색 성분 비율을 정수 형식으로 설정합니다. </summary>
		void IntG_set( int value );

		/// <summary> 파란색 성분 비율을 정수 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( int, IntB );

		/// <summary> 파란색 성분 비율을 정수 형식으로 가져옵니다. </summary>
		int IntB_get() const;

		/// <summary> 파란색 성분 비율을 정수 형식으로 설정합니다. </summary>
		void IntB_set( int value );

		/// <summary> 알파 성분 비율을 정수 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( int, IntA );

		/// <summary> 알파 성분 비율을 정수 형식으로 가져옵니다. </summary>
		int IntA_get() const;

		/// <summary> 알파 성분 비율을 정수 형식으로 설정합니다. </summary>
		void IntA_set( int value );

		Color& operator=( const Color& copy );
		Color& operator=( uint32 rgb );
		Color& operator+=( float value );
		Color& operator-=( float value );
		Color& operator*=( float value );
		Color& operator/=( float value );
		Color& operator+=( const Color& copy );
		Color& operator-=( const Color& copy );
		Color& operator*=( const Color& copy );
		Color& operator/=( const Color& copy );

		Color operator+( float value ) const;
		Color operator-( float value ) const;
		Color operator*( float value ) const;
		Color operator/( float value ) const;
		Color operator+( const Color& copy ) const;
		Color operator-( const Color& copy ) const;
		Color operator*( const Color& copy ) const;
		Color operator/( const Color& copy ) const;

		bool operator==( const Color& right ) const;
		bool operator!=( const Color& right ) const;
		bool operator< ( const Color& right ) const;

		/// <summary> HTML 코드 텍스트 형식에서 색상 값을 가져옵니다. </summary>
		static Color TryParse( String argbCode );
		
		/// <summary> RGB 값이 #F0F8FF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color AliceBlue;

		/// <summary> RGB 값이 #FAEBD7인 시스템 정의 색을 가져옵니다. </summary>
		static const Color AntiqueWhite;

		/// <summary> RGB 값이 #00FFFF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Aqua;

		/// <summary> RGB 값이 #7FFFD4인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Aquamarine;

		/// <summary> RGB 값이 #F0FFFF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Azure;

		/// <summary> RGB 값이 #F5F5DC인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Beige;

		/// <summary> RGB 값이 #FFE4C4인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Bisque;

		/// <summary> RGB 값이 #000000인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Black;

		/// <summary> RGB 값이 #FFEBCD인 시스템 정의 색을 가져옵니다. </summary>
		static const Color BlanchedAlmond;

		/// <summary> RGB 값이 #0000FF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Blue;

		/// <summary> RGB 값이 #8A2BE2인 시스템 정의 색을 가져옵니다. </summary>
		static const Color BlueViolet;

		/// <summary> RGB 값이 #A52A2A인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Brown;

		/// <summary> RGB 값이 #DEB887인 시스템 정의 색을 가져옵니다. </summary>
		static const Color BurlyWood;

		/// <summary> RGB 값이 #5F9EA0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color CadetBlue;

		/// <summary> RGB 값이 #7FFF00인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Chartreuse;

		/// <summary> RGB 값이 #D2691E인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Chocolate;

		/// <summary> RGB 값이 #FF7F50인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Coral;

		/// <summary> RGB 값이 #6495ED인 시스템 정의 색을 가져옵니다. </summary>
		static const Color CornflowerBlue;

		/// <summary> RGB 값이 #FFF8DC인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Cornsilk;

		/// <summary> RGB 값이 #DC143C인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Crimson;

		/// <summary> RGB 값이 #00FFFF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Cyan;

		/// <summary> RGB 값이 #00008B인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkBlue;

		/// <summary> RGB 값이 #008B8B인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkCyan;

		/// <summary> RGB 값이 #B8860B인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkGoldenrod;

		/// <summary> RGB 값이 #A9A9A9인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkGray;

		/// <summary> RGB 값이 #006400인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkGreen;

		/// <summary> RGB 값이 #BDB76B인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkKhaki;

		/// <summary> RGB 값이 #8B008B인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkMagenta;

		/// <summary> RGB 값이 #556B2F인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkOliveGreen;

		/// <summary> RGB 값이 #FF8C00인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkOrange;

		/// <summary> RGB 값이 #9932CC인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkOrchid;

		/// <summary> RGB 값이 #8B0000인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkRed;

		/// <summary> RGB 값이 #E9967A인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkSalmon;

		/// <summary> RGB 값이 #E9967A인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkSeaGreen;

		/// <summary> RGB 값이 #483D8B인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkSlateBlue;

		/// <summary> RGB 값이 #2F4F4F인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkSlateGray;

		/// <summary> RGB 값이 #00CED1인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkTurquoise;

		/// <summary> RGB 값이 #9400D3인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DarkViolet;

		/// <summary> RGB 값이 #FF1493인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DeepPink;

		/// <summary> RGB 값이 #00BFFF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DeepSkyBlue;

		/// <summary> RGB 값이 #696969인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DimGray;

		/// <summary> RGB 값이 #1E90FF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color DodgerBlue;

		/// <summary> RGB 값이 #B22222인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Firebrick;

		/// <summary> RGB 값이 #FFFAF0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color FloralWhite;

		/// <summary> RGB 값이 #228B22인 시스템 정의 색을 가져옵니다. </summary>
		static const Color ForestGreen;

		/// <summary> RGB 값이 #FF00FF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Fuchsia;

		/// <summary> RGB 값이 #DCDCDC인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Gainsboro;

		/// <summary> RGB 값이 #F8F8FF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color GhostWhite;

		/// <summary> RGB 값이 #FFD700인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Gold;

		/// <summary> RGB 값이 #DAA520인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Goldenrod;

		/// <summary> RGB 값이 #808080인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Gray;

		/// <summary> RGB 값이 #008000인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Green;

		/// <summary> RGB 값이 #ADFF2F인 시스템 정의 색을 가져옵니다. </summary>
		static const Color GreenYellow;

		/// <summary> RGB 값이 #F0FFF0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Honeydew;

		/// <summary> RGB 값이 #FF69B4인 시스템 정의 색을 가져옵니다. </summary>
		static const Color HotPink;

		/// <summary> RGB 값이 #CD5C5C인 시스템 정의 색을 가져옵니다. </summary>
		static const Color IndianRed;

		/// <summary> RGB 값이 #4B0082인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Indigo;

		/// <summary> RGB 값이 #FFFFF0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Ivory;

		/// <summary> RGB 값이 #F0E68C인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Khaki;

		/// <summary> RGB 값이 #E6E6FA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Lavender;

		/// <summary> RGB 값이 #FFF0F5인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LavenderBlush;

		/// <summary> RGB 값이 #7CFC00인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LawnGreen;

		/// <summary> RGB 값이 #FFFACD인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LemonChiffon;

		/// <summary> RGB 값이 #ADD8E6인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightBlue;

		/// <summary> RGB 값이 #F08080인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightCoral;

		/// <summary> RGB 값이 #E0FFFF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightCyan;

		/// <summary> RGB 값이 #FAFAD2인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightGoldenrodYellow;

		/// <summary> RGB 값이 #D3D3D3인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightGray;

		/// <summary> RGB 값이 #90EE90인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightGreen;

		/// <summary> RGB 값이 #FFB6C1인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightPink;

		/// <summary> RGB 값이 #FFA07A인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightSalmon;

		/// <summary> RGB 값이 #20B2AA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightSeaGreen;

		/// <summary> RGB 값이 #87CEFA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightSkyBlue;

		/// <summary> RGB 값이 #778899인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightSlateGray;

		/// <summary> RGB 값이 #B0C4DE인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightSteelBlue;

		/// <summary> RGB 값이 #FFFFE0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LightYellow;

		/// <summary> RGB 값이 #00FF00인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Lime;

		/// <summary> RGB 값이 #32CD32인 시스템 정의 색을 가져옵니다. </summary>
		static const Color LimeGreen;

		/// <summary> RGB 값이 #FAF0E6인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Linen;

		/// <summary> RGB 값이 #FF00FF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Magenta;

		/// <summary> RGB 값이 #800000인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Maroon;

		/// <summary> RGB 값이 #66CDAA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumAquamarine;

		/// <summary> RGB 값이 #0000CD인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumBlue;

		/// <summary> RGB 값이 #BA55D3인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumOrchid;

		/// <summary> RGB 값이 #9370DB인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumPurple;

		/// <summary> RGB 값이 #3CB371인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumSeaGreen;

		/// <summary> RGB 값이 #7B68EE인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumSlateBlue;

		/// <summary> RGB 값이 #00FA9A인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumSpringGreen;

		/// <summary> RGB 값이 #48D1CC인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumTurquoise;

		/// <summary> RGB 값이 #C71585인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MediumVioletRed;

		/// <summary> RGB 값이 #191970인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MidnightBlue;

		/// <summary> RGB 값이 #F5FFFA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MintCream;

		/// <summary> RGB 값이 #FFE4E1인 시스템 정의 색을 가져옵니다. </summary>
		static const Color MistyRose;

		/// <summary> RGB 값이 #FFE4B5인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Moccasin;

		/// <summary> RGB 값이 #FFDEAD인 시스템 정의 색을 가져옵니다. </summary>
		static const Color NavajoWhite;

		/// <summary> RGB 값이 #000080인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Navy;

		/// <summary> RGB 값이 #FDF5E6인 시스템 정의 색을 가져옵니다. </summary>
		static const Color OldLace;

		/// <summary> RGB 값이 #808000인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Olive;

		/// <summary> RGB 값이 #6B8E23인 시스템 정의 색을 가져옵니다. </summary>
		static const Color OliveDrab;

		/// <summary> RGB 값이 #FFA500인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Orange;

		/// <summary> RGB 값이 #FF4500인 시스템 정의 색을 가져옵니다. </summary>
		static const Color OrangeRed;

		/// <summary> RGB 값이 #DA70D6인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Orchid;

		/// <summary> RGB 값이 #EEE8AA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PaleGoldenrod;

		/// <summary> RGB 값이 #98FB98인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PaleGreen;

		/// <summary> RGB 값이 #AFEEEE인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PaleTurquoise;

		/// <summary> RGB 값이 #DB7093인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PaleVioletRed;

		/// <summary> RGB 값이 #FFEFD5인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PapayaWhip;

		/// <summary> RGB 값이 #FFDAB9인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PeachPuff;

		/// <summary> RGB 값이 #CD853F인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Peru;

		/// <summary> RGB 값이 #FFC0CB인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Pink;

		/// <summary> RGB 값이 #DDA0DD인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Plum;

		/// <summary> RGB 값이 #B0E0E6인 시스템 정의 색을 가져옵니다. </summary>
		static const Color PowderBlue;

		/// <summary> RGB 값이 #800080인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Purple;

		/// <summary> RGB 값이 #FF0000인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Red;

		/// <summary> RGB 값이 #BC8F8F인 시스템 정의 색을 가져옵니다. </summary>
		static const Color RosyBrown;

		/// <summary> RGB 값이 #4169E1인 시스템 정의 색을 가져옵니다. </summary>
		static const Color RoyalBlue;

		/// <summary> RGB 값이 #8B4513인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SaddleBrown;

		/// <summary> RGB 값이 #FA8072인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Salmon;

		/// <summary> RGB 값이 #F4A460인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SandyBrown;

		/// <summary> RGB 값이 #2E8B57인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SeaGreen;

		/// <summary> RGB 값이 #FFF5EE인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SeaShell;

		/// <summary> RGB 값이 #A0522D인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Sienna;

		/// <summary> RGB 값이 #C0C0C0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Silver;

		/// <summary> RGB 값이 #87CEEB인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SkyBlue;

		/// <summary> RGB 값이 #6A5ACD인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SlateBlue;

		/// <summary> RGB 값이 #708090인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SlateGray;

		/// <summary> RGB 값이 #FFFAFA인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Snow;

		/// <summary> RGB 값이 #00FF7F인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SpringGreen;

		/// <summary> RGB 값이 #4682B4인 시스템 정의 색을 가져옵니다. </summary>
		static const Color SteelBlue;

		/// <summary> RGB 값이 #D2B48C인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Tan;

		/// <summary> RGB 값이 #D2B48C인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Teal;

		/// <summary> RGB 값이 #D8BFD8인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Thistle;

		/// <summary> RGB 값이 #FF6347인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Tomato;

		/// <summary> RGB 값이 #40E0D0인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Turquoise;

		/// <summary> RGB 값이 #EE82EE인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Violet;

		/// <summary> RGB 값이 #F5DEB3인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Wheat;

		/// <summary> RGB 값이 #FFFFFF인 시스템 정의 색을 가져옵니다. </summary>
		static const Color White;

		/// <summary> RGB 값이 #F5F5F5인 시스템 정의 색을 가져옵니다. </summary>
		static const Color WhiteSmoke;

		/// <summary> RGB 값이 #FFFF00인 시스템 정의 색을 가져옵니다. </summary>
		static const Color Yellow;

		/// <summary> RGB 값이 #9ACD32인 시스템 정의 색을 가져옵니다. </summary>
		static const Color YellowGreen;
	};
}