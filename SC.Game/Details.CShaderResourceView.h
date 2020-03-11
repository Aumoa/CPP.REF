#pragma once

namespace SC::Game::Details
{
	class CShaderResourceView : public CView
	{
	public:
		CShaderResourceView( RefPtr<ViewStorage>& viewStorage, int lockIndex, D3D12_CPU_DESCRIPTOR_HANDLE handle );
	};
}