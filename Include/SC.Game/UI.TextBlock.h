#pragma once

namespace SC::Game::UI
{
	/// <summary> ������ �ؽ�Ʈ �޽����� ǥ���ϴ� UI ����Դϴ�. </summary>
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
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="TextBlock"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		TextBlock( String name );
		~TextBlock() override;

		/// <summary> (Visual Studio ����) �ؽ�Ʈ ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<TextFormat>, Format );

		/// <summary> �ؽ�Ʈ ������ �����ɴϴ�. </summary>
		RefPtr<TextFormat> Format_get();

		/// <summary> �ؽ�Ʈ ������ �����մϴ�. </summary>
		void Format_set( RefPtr<TextFormat> value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ ���� ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( TextAlignment, Alignment );

		/// <summary> �ؽ�Ʈ ���� ������ �����ɴϴ�. </summary>
		TextAlignment Alignment_get();

		/// <summary> �ؽ�Ʈ ���� ������ �����մϴ�. </summary>
		void Alignment_set( TextAlignment value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ ���� ���� ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( TextVerticalAlignment, VerticalAlignment );

		/// <summary> �ؽ�Ʈ ���� ���� ������ �����ɴϴ�. </summary>
		TextVerticalAlignment VerticalAlignment_get();

		/// <summary> �ؽ�Ʈ ���� ���� ������ �����մϴ�. </summary>
		void VerticalAlignment_set( TextVerticalAlignment value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ ũ�⿡ ���� ����� ũ�Ⱑ �ڵ����� �����ǵ��� �ϴ� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, AutoSizing );

		/// <summary> �ؽ�Ʈ ũ�⿡ ���� ����� ũ�Ⱑ �ڵ����� �����ǵ��� �ϴ� ���� �����ɴϴ�. </summary>
		bool AutoSizing_get();

		/// <summary> �ؽ�Ʈ ũ�⿡ ���� ����� ũ�Ⱑ �ڵ����� �����ǵ��� �ϴ� ���� �����մϴ�. </summary>
		void AutoSizing_set( bool value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ ä��� ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( Drawing::Color, Color );

		/// <summary> �ؽ�Ʈ ä��� �귯�ø� �����ɴϴ�. </summary>
		Drawing::Color Color_get();

		/// <summary> �ؽ�Ʈ ä��� �귯�ø� �����մϴ�. </summary>
		void Color_set( Drawing::Color value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ�� �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Opacity );

		/// <summary> �ؽ�Ʈ�� �������� �����ɴϴ�. </summary>
		double Opacity_get();

		/// <summary> �ؽ�Ʈ�� �������� �����մϴ�. </summary>
		void Opacity_set( double value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ�� RichText ������ ����ϴ��� ���θ� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsRichText );

		/// <summary> �ؽ�Ʈ�� RichText ������ ����ϴ��� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsRichText_get();

		/// <summary> �ؽ�Ʈ�� RichText ������ ����ϴ��� ���θ� ��Ÿ���� ���� �����մϴ�. </summary>
		void IsRichText_set( bool value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ�� �ܰ��� ũ�⸦ �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, OutlineThickness );

		/// <summary> �ؽ�Ʈ�� �ܰ��� ũ�⸦ �����ɴϴ�. </summary>
		double OutlineThickness_get();

		/// <summary> �ؽ�Ʈ�� �ܰ��� ũ�⸦ �����մϴ�. </summary>
		void OutlineThickness_set( double value );

		/// <summary> (Visual Studio ����) �ؽ�Ʈ�� �ܰ��� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( Drawing::Color, OutlineColor );

		/// <summary> �ؽ�Ʈ�� �ܰ��� ���� �����ɴϴ�. </summary>
		Drawing::Color OutlineColor_get();

		/// <summary> �ؽ�Ʈ�� �ܰ��� ���� �����մϴ�. </summary>
		void OutlineColor_set( Drawing::Color value );

	private:
		void OnFormatChanged();
		void OnContentChanged( object sender, object content );

		void ParseText( String original );
		void ForEachTokens( String token, std::function<bool( String key, String value )> lambda );
	};
}