#pragma once

namespace SC::Game::Details
{
	class IntegratedFactory : virtual public Object
	{
	public:
		IntegratedFactory();

		ComPtr<IDXGIAdapter1> SearchHardwareAdapter();

		ComPtr<IDXGIFactory6> pDXGIFactory;
		ComPtr<IDWriteFactory> pDWriteFactory;
		ComPtr<IWICImagingFactory> pWICFactory;
	};
}