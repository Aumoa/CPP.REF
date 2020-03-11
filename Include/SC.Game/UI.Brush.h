#pragma once

namespace SC::Game::UI
{
	/// <summary> �׷��� ��ü�� �׸��� �� ����� ��ü�� �����մϴ�. �Ļ��� Ŭ������ ������ �׸��� ����� �����մϴ�. </summary>
	class Brush : virtual public Object
	{
#if defined( __SC_GAME_EXPORT_SYMBOL__ )
		friend class Details::GlyphRenderer;
#endif
		friend class UI::Shapes::Rectangle;
		friend class UI::Border;

	protected:
#pragma pack( push, 4 )
		struct alignas( 256 ) Constants
		{
			int Type;
			float Opacity;
			float padding[2];
			sc_game_export_float( XMFLOAT4, 4 ) Attribute0;
			sc_game_export_float( XMFLOAT4, 4 ) Attribute1;
		};
#pragma pack( pop )

		sc_game_export_object( RefPtr<Details::CDynamicBuffer> ) brushConstants;

	protected:
		Brush( int type );
		~Brush() override;

		Constants* frameResource;

	public:
		/// <summary> (Visual Studio ����) �귯�� ��ü�� ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Opacity );

		/// <summary> �귯�� ��ü�� ������ �����ɴϴ�. </summary>
		double Opacity_get();

		/// <summary> �귯�� ��ü�� ������ �����մϴ�. </summary>
		void Opacity_set( double value );
	};
}