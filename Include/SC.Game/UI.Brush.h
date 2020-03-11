#pragma once

namespace SC::Game::UI
{
	/// <summary> 그래픽 개체를 그리는 데 사용할 개체를 정의합니다. 파생된 클래스는 영역을 그리는 방법을 설명합니다. </summary>
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
		/// <summary> (Visual Studio 전용) 브러시 개체의 투명도를 설정하거나 가져옵니다. </summary>
		vs_property( double, Opacity );

		/// <summary> 브러시 개체의 투명도를 가져옵니다. </summary>
		double Opacity_get();

		/// <summary> 브러시 개체의 투명도를 설정합니다. </summary>
		void Opacity_set( double value );
	};
}