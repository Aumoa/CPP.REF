#pragma once

namespace SC::Game::Details
{
	class CDeviceContext : virtual public Object
	{
		struct tag_WaitPair
		{
			ID3D12Fence* pFence;
			uint64 FenceValue;
		};

		const D3D12_COMMAND_LIST_TYPE type;
		ComPtr<ID3D12Device> pDevice;

		RefPtr<VisibleViewStorage> viewStorage;
		std::map<std::string, int, std::less<>> slotMap;

		RefPtr<CDynamicBuffer> uiShaderInfos[2];
		uint32 uiShaderInfoOffset = 0;

	public:
		CDeviceContext( RefPtr<CDevice>& device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* pInitialCommandAllocator = nullptr, ID3D12PipelineState* pInitialPipelineState = nullptr );

		void CreateShaderInfoBuffers( int capacity = 2048 );
		
		void Reset( CCommandQueue* pCommandQueue, ID3D12CommandAllocator* pAllocator, ID3D12PipelineState* pInitialPipelineState = nullptr );
		void Close();
		void SetVisibleViewStorage( RefPtr<VisibleViewStorage>& srvStorage );
		void SetSlotMap( const std::map<std::string, int, std::less<>>& slotMap );
		void SetGraphicsRootShaderResources( int slot, RefPtr<CShaderResourceView>& pSRV );
		void SetGraphicsRootShaderResources( int slot, int numViews, CShaderResourceView* const* ppSRVs );
		void SetGraphicsRootShaderResources( int slot, int numViews, RefPtr<CShaderResourceView>* ppSRVs );
		void SetComputeRootShaderResources( int slot, RefPtr<CShaderResourceView>& pSRV );
		void SetComputeRootShaderResources( int slot, int numViews, CShaderResourceView* const* ppSRVs );
		void SetComputeRootShaderResources( int slot, int numViews, RefPtr<CShaderResourceView>* ppSRVs );
		void SetComputeRootUnorderedAccessViews( int slot, RefPtr<CUnorderedAccessView>& pUAV );
		void SetComputeRootUnorderedAccessViews( int slot, int numViews, CUnorderedAccessView* const* ppUAVs );
		void SetComputeRootUnorderedAccessViews( int slot, int numViews, RefPtr<CUnorderedAccessView>* ppUAVs );
		void DispatchShaderInfo( int numStructs, const tag_ShaderInfo* pShaderInfos );

		CCommandQueue* pCommandQueue = nullptr;
		ComPtr<ID3D12GraphicsCommandList> pCommandList;
		bool Failure = false;

		vs_property_get( int, Slot )[];
		int Slot_get( const std::string_view& param0 );

		template< int Count >
		void TransitionBarrier( ID3D12Resource* ( &ppResources )[Count], D3D12_RESOURCE_STATES( &pStatesBefore )[Count], D3D12_RESOURCE_STATES( &pStatesAfter )[Count], int( &pSubresourceIndexes )[Count] )
		{
			D3D12_RESOURCE_BARRIER barriers[Count]{ };

			for ( int i = 0; i < Count; ++i )
			{
				barriers[i].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[i].Transition.pResource = ppResources[i];
				barriers[i].Transition.StateBefore = pStatesBefore[i];
				barriers[i].Transition.StateAfter = pStatesAfter[i];
				if ( pSubresourceIndexes ) barriers[i].Transition.Subresource = pSubresourceIndexes[i];
			}

			pCommandList->ResourceBarrier( Count, barriers );
		}

		template< class... _Args >
		void TransitionBarrier( ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, int SubresourceIndex, _Args... args )
		{
			constexpr int sizeOf = 1 + sizeof...( _Args ) / 4;
			ID3D12Resource* ppResources[sizeOf];
			D3D12_RESOURCE_STATES pBeforeStates[sizeOf];
			D3D12_RESOURCE_STATES pAfterStates[sizeOf];
			int pSubresourceIndexes[sizeOf];

			TransitionBarrier_Helper( ppResources, pBeforeStates, pAfterStates, pSubresourceIndexes, pResource, StateBefore, StateAfter, SubresourceIndex, args... );

			TransitionBarrier( ppResources, pBeforeStates, pAfterStates, pSubresourceIndexes );
		}

	private:
		template< class... _Args >
		inline void TransitionBarrier_Helper( ID3D12Resource** ppResources, D3D12_RESOURCE_STATES* pBeforeStates, D3D12_RESOURCE_STATES* pAfterStates, int* pSubresourceIndexes, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, int SubresourceIndex, _Args... args )
		{
			*ppResources++ = pResource;
			*pBeforeStates++ = StateBefore;
			*pAfterStates++ = StateAfter;
			*pSubresourceIndexes++ = SubresourceIndex;

			TransitionBarrier_Helper( ppResources, pBeforeStates, pAfterStates, pSubresourceIndexes, args... );
		}

		inline void TransitionBarrier_Helper( ID3D12Resource** ppResources, D3D12_RESOURCE_STATES* pBeforeStates, D3D12_RESOURCE_STATES* pAfterStates, int* pSubresourceIndexes, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, int SubresourceIndex )
		{
			*ppResources++ = pResource;
			*pBeforeStates++ = StateBefore;
			*pAfterStates++ = StateAfter;
			*pSubresourceIndexes++ = SubresourceIndex;
		}
	};
}