#pragma once

namespace SC::Drawing
{
	/// <summary> 4���� �ε� �Ҽ��� ������ ����Ͽ� ���� ������ ǥ���մϴ�. </summary>
	struct Color : public ValueType, virtual public IComparable<Color>, virtual public IEquatable<Color>
	{
		/// <summary> ������ ���� ������ �����ϰų� �����ɴϴ�. </summary>
		float R;

		/// <summary> �ʷϻ� ���� ������ �����ϰų� �����ɴϴ�. </summary>
		float G;

		/// <summary> �Ķ��� ���� ������ �����ϰų� �����ɴϴ�. </summary>
		float B;

		/// <summary> ���� ���� ������ �����ϰų� �����ɴϴ�. </summary>
		float A;

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Color();

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Color( const Color& copy );

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="rgb"> RGB ���� ������ ���� ��ü�� �����մϴ�. </param>
		/// <param name="alphaOverride"> ��� ���ο� ���� ���� �����մϴ�. </param>
		Color( const Color& rgb, float alphaOverride );

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Color( float r, float g, float b, float a = 1.0f );

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Color( int r, int g, int b, int a = 255 );

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="rgb"> RGB ���� �ڵ带 �����մϴ�. </param>
		/// <param name="a"> ���İ��� �����մϴ�. </param>
		Color( uint32 rgb, int a );

		/// <summary> <see cref="Color"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="rgb"> RGB ���� �ڵ带 �����մϴ�. </param>
		/// <param name="a"> ���İ��� �����մϴ�. </param>
		Color( uint32 rgb, float a = 1.0f );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ #ARGB �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( Color right ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		int CompareTo( Color right ) override;

		/// <summary> �� ���� ���� ���� �Լ��� �����ϴ�. </summary>
		Color AlphaBlend( const Color& right ) const;

		/// <summary> ������ ���� ������ ���� �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, IntR );

		/// <summary> ������ ���� ������ ���� �������� �����ɴϴ�. </summary>
		int IntR_get() const;

		/// <summary> ������ ���� ������ ���� �������� �����մϴ�. </summary>
		void IntR_set( int value );

		/// <summary> �ʷϻ� ���� ������ ���� �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, IntG );

		/// <summary> �ʷϻ� ���� ������ ���� �������� �����ɴϴ�. </summary>
		int IntG_get() const;

		/// <summary> �ʷϻ� ���� ������ ���� �������� �����մϴ�. </summary>
		void IntG_set( int value );

		/// <summary> �Ķ��� ���� ������ ���� �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, IntB );

		/// <summary> �Ķ��� ���� ������ ���� �������� �����ɴϴ�. </summary>
		int IntB_get() const;

		/// <summary> �Ķ��� ���� ������ ���� �������� �����մϴ�. </summary>
		void IntB_set( int value );

		/// <summary> ���� ���� ������ ���� �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, IntA );

		/// <summary> ���� ���� ������ ���� �������� �����ɴϴ�. </summary>
		int IntA_get() const;

		/// <summary> ���� ���� ������ ���� �������� �����մϴ�. </summary>
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

		/// <summary> HTML �ڵ� �ؽ�Ʈ ���Ŀ��� ���� ���� �����ɴϴ�. </summary>
		static Color TryParse( String argbCode );
		
		/// <summary> RGB ���� #F0F8FF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color AliceBlue;

		/// <summary> RGB ���� #FAEBD7�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color AntiqueWhite;

		/// <summary> RGB ���� #00FFFF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Aqua;

		/// <summary> RGB ���� #7FFFD4�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Aquamarine;

		/// <summary> RGB ���� #F0FFFF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Azure;

		/// <summary> RGB ���� #F5F5DC�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Beige;

		/// <summary> RGB ���� #FFE4C4�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Bisque;

		/// <summary> RGB ���� #000000�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Black;

		/// <summary> RGB ���� #FFEBCD�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color BlanchedAlmond;

		/// <summary> RGB ���� #0000FF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Blue;

		/// <summary> RGB ���� #8A2BE2�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color BlueViolet;

		/// <summary> RGB ���� #A52A2A�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Brown;

		/// <summary> RGB ���� #DEB887�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color BurlyWood;

		/// <summary> RGB ���� #5F9EA0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color CadetBlue;

		/// <summary> RGB ���� #7FFF00�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Chartreuse;

		/// <summary> RGB ���� #D2691E�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Chocolate;

		/// <summary> RGB ���� #FF7F50�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Coral;

		/// <summary> RGB ���� #6495ED�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color CornflowerBlue;

		/// <summary> RGB ���� #FFF8DC�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Cornsilk;

		/// <summary> RGB ���� #DC143C�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Crimson;

		/// <summary> RGB ���� #00FFFF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Cyan;

		/// <summary> RGB ���� #00008B�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkBlue;

		/// <summary> RGB ���� #008B8B�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkCyan;

		/// <summary> RGB ���� #B8860B�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkGoldenrod;

		/// <summary> RGB ���� #A9A9A9�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkGray;

		/// <summary> RGB ���� #006400�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkGreen;

		/// <summary> RGB ���� #BDB76B�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkKhaki;

		/// <summary> RGB ���� #8B008B�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkMagenta;

		/// <summary> RGB ���� #556B2F�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkOliveGreen;

		/// <summary> RGB ���� #FF8C00�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkOrange;

		/// <summary> RGB ���� #9932CC�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkOrchid;

		/// <summary> RGB ���� #8B0000�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkRed;

		/// <summary> RGB ���� #E9967A�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkSalmon;

		/// <summary> RGB ���� #E9967A�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkSeaGreen;

		/// <summary> RGB ���� #483D8B�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkSlateBlue;

		/// <summary> RGB ���� #2F4F4F�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkSlateGray;

		/// <summary> RGB ���� #00CED1�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkTurquoise;

		/// <summary> RGB ���� #9400D3�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DarkViolet;

		/// <summary> RGB ���� #FF1493�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DeepPink;

		/// <summary> RGB ���� #00BFFF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DeepSkyBlue;

		/// <summary> RGB ���� #696969�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DimGray;

		/// <summary> RGB ���� #1E90FF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color DodgerBlue;

		/// <summary> RGB ���� #B22222�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Firebrick;

		/// <summary> RGB ���� #FFFAF0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color FloralWhite;

		/// <summary> RGB ���� #228B22�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color ForestGreen;

		/// <summary> RGB ���� #FF00FF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Fuchsia;

		/// <summary> RGB ���� #DCDCDC�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Gainsboro;

		/// <summary> RGB ���� #F8F8FF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color GhostWhite;

		/// <summary> RGB ���� #FFD700�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Gold;

		/// <summary> RGB ���� #DAA520�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Goldenrod;

		/// <summary> RGB ���� #808080�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Gray;

		/// <summary> RGB ���� #008000�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Green;

		/// <summary> RGB ���� #ADFF2F�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color GreenYellow;

		/// <summary> RGB ���� #F0FFF0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Honeydew;

		/// <summary> RGB ���� #FF69B4�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color HotPink;

		/// <summary> RGB ���� #CD5C5C�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color IndianRed;

		/// <summary> RGB ���� #4B0082�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Indigo;

		/// <summary> RGB ���� #FFFFF0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Ivory;

		/// <summary> RGB ���� #F0E68C�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Khaki;

		/// <summary> RGB ���� #E6E6FA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Lavender;

		/// <summary> RGB ���� #FFF0F5�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LavenderBlush;

		/// <summary> RGB ���� #7CFC00�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LawnGreen;

		/// <summary> RGB ���� #FFFACD�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LemonChiffon;

		/// <summary> RGB ���� #ADD8E6�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightBlue;

		/// <summary> RGB ���� #F08080�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightCoral;

		/// <summary> RGB ���� #E0FFFF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightCyan;

		/// <summary> RGB ���� #FAFAD2�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightGoldenrodYellow;

		/// <summary> RGB ���� #D3D3D3�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightGray;

		/// <summary> RGB ���� #90EE90�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightGreen;

		/// <summary> RGB ���� #FFB6C1�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightPink;

		/// <summary> RGB ���� #FFA07A�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightSalmon;

		/// <summary> RGB ���� #20B2AA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightSeaGreen;

		/// <summary> RGB ���� #87CEFA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightSkyBlue;

		/// <summary> RGB ���� #778899�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightSlateGray;

		/// <summary> RGB ���� #B0C4DE�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightSteelBlue;

		/// <summary> RGB ���� #FFFFE0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LightYellow;

		/// <summary> RGB ���� #00FF00�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Lime;

		/// <summary> RGB ���� #32CD32�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color LimeGreen;

		/// <summary> RGB ���� #FAF0E6�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Linen;

		/// <summary> RGB ���� #FF00FF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Magenta;

		/// <summary> RGB ���� #800000�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Maroon;

		/// <summary> RGB ���� #66CDAA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumAquamarine;

		/// <summary> RGB ���� #0000CD�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumBlue;

		/// <summary> RGB ���� #BA55D3�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumOrchid;

		/// <summary> RGB ���� #9370DB�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumPurple;

		/// <summary> RGB ���� #3CB371�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumSeaGreen;

		/// <summary> RGB ���� #7B68EE�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumSlateBlue;

		/// <summary> RGB ���� #00FA9A�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumSpringGreen;

		/// <summary> RGB ���� #48D1CC�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumTurquoise;

		/// <summary> RGB ���� #C71585�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MediumVioletRed;

		/// <summary> RGB ���� #191970�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MidnightBlue;

		/// <summary> RGB ���� #F5FFFA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MintCream;

		/// <summary> RGB ���� #FFE4E1�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color MistyRose;

		/// <summary> RGB ���� #FFE4B5�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Moccasin;

		/// <summary> RGB ���� #FFDEAD�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color NavajoWhite;

		/// <summary> RGB ���� #000080�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Navy;

		/// <summary> RGB ���� #FDF5E6�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color OldLace;

		/// <summary> RGB ���� #808000�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Olive;

		/// <summary> RGB ���� #6B8E23�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color OliveDrab;

		/// <summary> RGB ���� #FFA500�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Orange;

		/// <summary> RGB ���� #FF4500�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color OrangeRed;

		/// <summary> RGB ���� #DA70D6�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Orchid;

		/// <summary> RGB ���� #EEE8AA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PaleGoldenrod;

		/// <summary> RGB ���� #98FB98�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PaleGreen;

		/// <summary> RGB ���� #AFEEEE�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PaleTurquoise;

		/// <summary> RGB ���� #DB7093�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PaleVioletRed;

		/// <summary> RGB ���� #FFEFD5�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PapayaWhip;

		/// <summary> RGB ���� #FFDAB9�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PeachPuff;

		/// <summary> RGB ���� #CD853F�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Peru;

		/// <summary> RGB ���� #FFC0CB�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Pink;

		/// <summary> RGB ���� #DDA0DD�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Plum;

		/// <summary> RGB ���� #B0E0E6�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color PowderBlue;

		/// <summary> RGB ���� #800080�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Purple;

		/// <summary> RGB ���� #FF0000�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Red;

		/// <summary> RGB ���� #BC8F8F�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color RosyBrown;

		/// <summary> RGB ���� #4169E1�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color RoyalBlue;

		/// <summary> RGB ���� #8B4513�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SaddleBrown;

		/// <summary> RGB ���� #FA8072�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Salmon;

		/// <summary> RGB ���� #F4A460�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SandyBrown;

		/// <summary> RGB ���� #2E8B57�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SeaGreen;

		/// <summary> RGB ���� #FFF5EE�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SeaShell;

		/// <summary> RGB ���� #A0522D�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Sienna;

		/// <summary> RGB ���� #C0C0C0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Silver;

		/// <summary> RGB ���� #87CEEB�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SkyBlue;

		/// <summary> RGB ���� #6A5ACD�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SlateBlue;

		/// <summary> RGB ���� #708090�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SlateGray;

		/// <summary> RGB ���� #FFFAFA�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Snow;

		/// <summary> RGB ���� #00FF7F�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SpringGreen;

		/// <summary> RGB ���� #4682B4�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color SteelBlue;

		/// <summary> RGB ���� #D2B48C�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Tan;

		/// <summary> RGB ���� #D2B48C�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Teal;

		/// <summary> RGB ���� #D8BFD8�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Thistle;

		/// <summary> RGB ���� #FF6347�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Tomato;

		/// <summary> RGB ���� #40E0D0�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Turquoise;

		/// <summary> RGB ���� #EE82EE�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Violet;

		/// <summary> RGB ���� #F5DEB3�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Wheat;

		/// <summary> RGB ���� #FFFFFF�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color White;

		/// <summary> RGB ���� #F5F5F5�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color WhiteSmoke;

		/// <summary> RGB ���� #FFFF00�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color Yellow;

		/// <summary> RGB ���� #9ACD32�� �ý��� ���� ���� �����ɴϴ�. </summary>
		static const Color YellowGreen;
	};
}