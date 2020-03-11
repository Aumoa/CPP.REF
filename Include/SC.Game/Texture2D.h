#pragma once

namespace SC::Game
{
	/// <summary> 2차원 텍스처 데이터를 표현합니다. </summary>
	class Texture2D : public Assets
	{
		friend class Material;
		friend class Details::GameLogic;
		friend class UI::Image;

		sc_game_export_object( ComPtr<ID3D12Resource> ) pTexture2D;
		sc_game_export_object( RefPtr<Details::CShaderResourceView> ) pShaderResourceView;

		sc_game_export_object( ComPtr<ID3D12CommandAllocator> ) pUploadCommands;
		sc_game_export_object( ComPtr<ID3D12Resource> ) pUploadHeap;
		uint64 uploadFenceValue = 0;

		bool copySuccessFlag = false;
		bool hasTransitionFlag = false;
		bool syncFlag = false;

		uint32 width;
		uint32 height;

		bool Lock( RefPtr<Details::CDeviceContext>& deviceContext, bool sync = true );
		
		vs_property_get( bool, IsValid );
		bool IsValid_get();
		
	public:
		~Texture2D() override;

		/// <summary> <see cref="Texture2D"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 제공합니다. </param>
		/// <param name="filepath"> 이미지 파일 경로를 전달합니다. </param>
		Texture2D( String name, const std::filesystem::path& filepath );

		/// <summary> <see cref="Texture2D"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 제공합니다. </param>
		/// <param name="textureData"> 텍스처 데이터를 전달합니다. </param>
		/// <param name="sizeInBytes"> 텍스처 데이터의 크기를 전달합니다. </param>
		Texture2D( String name, void* textureData, uint32 sizeInBytes );

		/// <summary> (Visual Studio 전용) 텍스처 개체의 픽셀 넓이를 가져옵니다. </summary>
		vs_property_get( uint32, Width );

		/// <summary> 텍스처 개체의 픽셀 넓이를 가져옵니다. </summary>
		uint32 Width_get();

		/// <summary> (Visual Studio 전용) 텍스처 개체의 픽셀 높이를 가져옵니다. </summary>
		vs_property_get( uint32, Height );

		/// <summary> 텍스처 개체의 픽셀 높이를 가져옵니다. </summary>
		uint32 Height_get();

	private:
		void InitializeFrom( sc_game_export_object( IWICBitmapDecoder* ) pDecoder );
	};
}