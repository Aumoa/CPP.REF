#pragma once

namespace SC::Game::Details
{
	class HDRBuffer : public RenderTexture2D
	{
		RefPtr<CDevice>& deviceRef;

		ComPtr<ID3D12Resource> pHDRBuffer;

		ComPtr<ID3D12DescriptorHeap> pRTVHeap;

		D3D12_VIEWPORT viewport{ };
		D3D12_RECT scissor{ };
		uint32 rtvStride = 0;

		uint32 width = 0;
		uint32 height = 0;

		RefPtr<CShaderResourceView> ppShaderResourceViews[1];

	public:
		HDRBuffer( RefPtr<CDevice>& device );

		void OMSetRenderTargets( RefPtr<CDeviceContext>& deviceContext );
		void SetGraphicsRootShaderResources( RefPtr<CDeviceContext>& deviceContext );
		void SetComputeRootShaderResources( RefPtr<CDeviceContext>& deviceContext );
		void EndDraw( RefPtr<CDeviceContext>& deviceContext );

		void ResizeBuffers( uint32 width, uint32 height );

		uint32 Width_get() override;
		uint32 Height_get() override;
	};
}