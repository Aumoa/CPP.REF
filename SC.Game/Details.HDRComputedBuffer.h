#pragma once

namespace SC::Game::Details
{
	class HDRComputedBuffer : virtual public Object
	{
		struct HDRConstants
		{
			float AvgLuminance;
		};

		RefPtr<CDevice>& deviceRef;

		ComPtr<ID3D12Resource> pHDRHalfBuffer;
		ComPtr<ID3D12Resource> pHDRConstants;
		Drawing::Point<int> halfSize;

		RefPtr<CUnorderedAccessView> pUnorderedAccessViews[2];

	public:
		HDRComputedBuffer( RefPtr<CDevice>& device );

		void SetComputeRootTimestamp( RefPtr<Details::CDeviceContext>& deviceContext, double time );
		void SetGraphicsRootConstantBuffers( RefPtr<Details::CDeviceContext>& deviceContext );
		void Compute( RefPtr<Details::CDeviceContext>& deviceContext, int pass );
		void ResizeBuffers( uint32 width, uint32 height );
	};
}