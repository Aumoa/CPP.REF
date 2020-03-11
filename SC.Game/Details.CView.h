#pragma once

namespace SC::Game::Details
{
	class CView : virtual public Object
	{
		RefPtr<ViewStorage> viewStorage;
		int lockIndex;

	public:
		CView( RefPtr<ViewStorage>& viewStorage, int lockIndex, D3D12_CPU_DESCRIPTOR_HANDLE handle );
		~CView() override;

		D3D12_CPU_DESCRIPTOR_HANDLE Handle;
	};
}