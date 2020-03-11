#pragma once

namespace SC::Game::Details
{
	struct tag_Rect
	{
		float left;
		float top;
		float width;
		float height;
	};

	struct tag_ShaderInfo
	{
		tag_Rect input;
		tag_Rect output;
	};
}