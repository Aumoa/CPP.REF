#pragma once

namespace SC::Game::UI
{
	/// <summary> ��Ʈ, �ؽ�Ʈ ���� ���� �����ϴ� �ؽ�Ʈ ���� ��ü�� ǥ���մϴ�. </summary>
	class TextFormat : virtual public Object
	{
		friend class TextBlock;

		String fontFamilyName;
		double fontSize = 0;

		sc_game_export_object( IDWriteTextFormat* ) pTextFormat;
		sc_game_export_object( RefPtr<Details::GlyphBuffer> ) glyphBuffer;

	public:
		/// <summary> <see cref="TextFormat"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="fontFamilyName"> ��Ʈ �йи� �̸��� �����մϴ�. �� �ؽ�Ʈ ���� �Ǵ� nullptr ���� �ùٸ��� ���� �ؽ�Ʈ�� ������ ��� �ý��� �⺻���� ���˴ϴ�. </param>
		/// <param name="fontSize"> ��Ʈ ũ�⸦ �����մϴ�. </param>
		TextFormat( String fontFamilyName, double fontSize = 15.0 );
		~TextFormat() override;
	};
}