#pragma once

namespace SC::Game::UI
{
	/// <summary> 고정된 텍스트 메시지를 표시하는 UI 요소입니다. </summary>
	class TextBlock : public Element
	{
		bool autoSizing = true;
		bool isRichText = false;

		RefPtr<TextFormat> textFormat;
		sc_game_export_object( ComPtr<IDWriteTextLayout> ) pLayout;
		sc_game_export_object( RefPtr<Details::GlyphRenderer> ) glyphRenderer;

		TextAlignment textAlignment = TextAlignment::Left;
		TextVerticalAlignment verticalAlignment = TextVerticalAlignment::Top;

		std::vector<object> richColors;

	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="TextBlock"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		TextBlock( String name );
		~TextBlock() override;

		/// <summary> (Visual Studio 전용) 텍스트 서식을 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<TextFormat>, Format );

		/// <summary> 텍스트 서식을 가져옵니다. </summary>
		RefPtr<TextFormat> Format_get();

		/// <summary> 텍스트 서식을 설정합니다. </summary>
		void Format_set( RefPtr<TextFormat> value );

		/// <summary> (Visual Studio 전용) 텍스트 정렬 형식을 설정하거나 가져옵니다. </summary>
		vs_property( TextAlignment, Alignment );

		/// <summary> 텍스트 정렬 형식을 가져옵니다. </summary>
		TextAlignment Alignment_get();

		/// <summary> 텍스트 정렬 형식을 설정합니다. </summary>
		void Alignment_set( TextAlignment value );

		/// <summary> (Visual Studio 전용) 텍스트 수직 정렬 형식을 설정하거나 가져옵니다. </summary>
		vs_property( TextVerticalAlignment, VerticalAlignment );

		/// <summary> 텍스트 수직 정렬 형식을 가져옵니다. </summary>
		TextVerticalAlignment VerticalAlignment_get();

		/// <summary> 텍스트 수직 정렬 형식을 설정합니다. </summary>
		void VerticalAlignment_set( TextVerticalAlignment value );

		/// <summary> (Visual Studio 전용) 텍스트 크기에 맞춰 요소의 크기가 자동으로 설정되도록 하는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, AutoSizing );

		/// <summary> 텍스트 크기에 맞춰 요소의 크기가 자동으로 설정되도록 하는 값을 가져옵니다. </summary>
		bool AutoSizing_get();

		/// <summary> 텍스트 크기에 맞춰 요소의 크기가 자동으로 설정되도록 하는 값을 설정합니다. </summary>
		void AutoSizing_set( bool value );

		/// <summary> (Visual Studio 전용) 텍스트 채우기 색상을 설정하거나 가져옵니다. </summary>
		vs_property( Drawing::Color, Color );

		/// <summary> 텍스트 채우기 브러시를 가져옵니다. </summary>
		Drawing::Color Color_get();

		/// <summary> 텍스트 채우기 브러시를 설정합니다. </summary>
		void Color_set( Drawing::Color value );

		/// <summary> (Visual Studio 전용) 텍스트의 불투명도를 설정하거나 가져옵니다. </summary>
		vs_property( double, Opacity );

		/// <summary> 텍스트의 불투명도를 가져옵니다. </summary>
		double Opacity_get();

		/// <summary> 텍스트의 불투명도를 설정합니다. </summary>
		void Opacity_set( double value );

		/// <summary> (Visual Studio 전용) 텍스트가 RichText 형식을 사용하는지 여부를 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsRichText );

		/// <summary> 텍스트가 RichText 형식을 사용하는지 여부를 나타내는 값을 가져옵니다. </summary>
		bool IsRichText_get();

		/// <summary> 텍스트가 RichText 형식을 사용하는지 여부를 나타내는 값을 설정합니다. </summary>
		void IsRichText_set( bool value );

		/// <summary> (Visual Studio 전용) 텍스트의 외각선 크기를 설정하거나 가져옵니다. </summary>
		vs_property( double, OutlineThickness );

		/// <summary> 텍스트의 외각선 크기를 가져옵니다. </summary>
		double OutlineThickness_get();

		/// <summary> 텍스트의 외각선 크기를 설정합니다. </summary>
		void OutlineThickness_set( double value );

		/// <summary> (Visual Studio 전용) 텍스트의 외각선 색을 설정하거나 가져옵니다. </summary>
		vs_property( Drawing::Color, OutlineColor );

		/// <summary> 텍스트의 외각선 색을 가져옵니다. </summary>
		Drawing::Color OutlineColor_get();

		/// <summary> 텍스트의 외각선 색을 설정합니다. </summary>
		void OutlineColor_set( Drawing::Color value );

	private:
		void OnFormatChanged();
		void OnContentChanged( object sender, object content );

		void ParseText( String original );
		void ForEachTokens( String token, std::function<bool( String key, String value )> lambda );
	};
}