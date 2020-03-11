#pragma once

namespace SC::Game
{
	/// <summary> 렌더링 대상으로 사용할 수 있는 2차원 텍스처 데이터를 표현합니다. </summary>
	class RenderTexture2D : virtual public Object
	{
	protected:
		RenderTexture2D();

	public:
		/// <summary> (Visual Studio 전용) 텍스처 버퍼의 픽셀 너비를 가져옵니다. </summary>
		vs_property_get( uint32, Width );

		/// <summary> 텍스처 버퍼의 픽셀 너비를 가져옵니다. </summary>
		virtual uint32 Width_get() = 0;

		/// <summary> (Visual Studio 전용) 텍스처 버퍼의 픽셀 높이를 가져옵니다. </summary>
		vs_property_get( uint32, Height );

		/// <summary> 텍스처 버퍼의 픽셀 높이를 가져옵니다. </summary>c
		virtual uint32 Height_get() = 0;
	};
}