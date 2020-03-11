#pragma once

namespace SC::Game::Details
{
	class GeometryBuffer : public RenderTexture2D
	{
		RefPtr<CDevice>& deviceRef;

		ComPtr<ID3D12Resource> pColorBuffer;
		ComPtr<ID3D12Resource> pNormalBuffer;
		ComPtr<ID3D12Resource> pMaterialBuffer;
		ComPtr<ID3D12Resource> pDepthBuffer;

		ComPtr<ID3D12DescriptorHeap> pRTVHeap;
		ComPtr<ID3D12DescriptorHeap> pDSVHeap;

		D3D12_VIEWPORT viewport{ };
		D3D12_RECT scissor{ };
		uint32 rtvStride = 0;

		uint32 width = 0;
		uint32 height = 0;

		RefPtr<CShaderResourceView> ppShaderResourceViews[4];

	public:
		GeometryBuffer( RefPtr<CDevice>& device );

		void OMSetRenderTargets( RefPtr<CDeviceContext>& deviceContext );
		void SetGraphicsRootShaderResourcesForColor( RefPtr<CDeviceContext>& deviceContext );
		void SetGraphicsRootShaderResourcesForLayer( RefPtr<CDeviceContext>& deviceContext );
		void EndDraw( RefPtr<CDeviceContext>& deviceContext );

		void ResizeBuffers( uint32 width, uint32 height );

		uint32 Width_get() override;
		uint32 Height_get() override;
	};
}