#pragma once

namespace SC::Game::Details
{
	class CUnorderedAccessView : public CView
	{
	public:
		CUnorderedAccessView( RefPtr<ViewStorage>& viewStorage, int lockIndex, D3D12_CPU_DESCRIPTOR_HANDLE handle );
	};
}