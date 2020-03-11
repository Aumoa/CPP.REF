using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

CDeviceContext::CDeviceContext( RefPtr<CDevice>& device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* pInitialCommandAllocator, ID3D12PipelineState* pInitialPipelineState )
	: pDevice( device->pDevice )
	, type( type )
{
	if ( pInitialCommandAllocator )
	{
		HR( pDevice->CreateCommandList( 0, type, pInitialCommandAllocator, pInitialPipelineState, IID_PPV_ARGS( &pCommandList ) ) );
	}
}

void CDeviceContext::CreateShaderInfoBuffers( int capacity )
{
	uiShaderInfos[0] = GlobalVar.device->CreateDynamicBuffer( sizeof( tag_ShaderInfo ) * capacity );  // 32KB
	uiShaderInfos[1] = GlobalVar.device->CreateDynamicBuffer( sizeof( tag_ShaderInfo ) * capacity );  // 32KB
}

void CDeviceContext::Reset( CCommandQueue* pCommandQueue, ID3D12CommandAllocator* pAllocator, ID3D12PipelineState* pInitialPipelineState )
{
	if ( !pCommandList )
	{
		HR( pDevice->CreateCommandList( 0, type, pAllocator, pInitialPipelineState, IID_PPV_ARGS( &pCommandList ) ) );
	}
	else
	{
		HR( pCommandList->Reset( pAllocator, pInitialPipelineState ) );
	}

	viewStorage = nullptr;
	slotMap.clear();
	Failure = false;

	uiShaderInfoOffset = 0;
	this->pCommandQueue = pCommandQueue;
}

void CDeviceContext::Close()
{
	this->pCommandQueue = nullptr;

#if defined( _DEBUG )
	if ( !pCommandList )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.Close(): pCommandList is Null." );
	}
#endif

	HR( pCommandList->Close() );
}

void CDeviceContext::SetVisibleViewStorage( RefPtr<VisibleViewStorage>& srvStorage )
{
	viewStorage = srvStorage;

	if ( srvStorage )
	{
		ID3D12DescriptorHeap* ppHeaps[]{ srvStorage->pDescriptorHeap.Get() };
		pCommandList->SetDescriptorHeaps( 1, ppHeaps );
	}
}

void CDeviceContext::SetSlotMap( const map<string, int, less<>>& slotMap )
{
	this->slotMap = slotMap;
}

void CDeviceContext::SetGraphicsRootShaderResources( int slot, RefPtr<CShaderResourceView>& pSRV )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetGraphicsRootShaderResources(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetGraphicsRootShaderResources(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	auto lock = viewStorage->Lock();
	if ( lock != -1 )
	{
		auto target = viewStorage->GetCPUHandle( lock );
		auto from = pSRV->Handle;
		base = viewStorage->GetGPUHandle( lock );

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
		pCommandList->SetGraphicsRootDescriptorTable( ( UINT )slot, base );
	}
	else
	{
		Failure = true;
	}
}

void CDeviceContext::SetGraphicsRootShaderResources( int slot, int numViews, CShaderResourceView* const* ppSRVs )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetGraphicsRootShaderResources(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetGraphicsRootShaderResources(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	for ( int i = 0; i < numViews; ++i )
	{
		auto lock = viewStorage->Lock();
		if ( lock == -1 )
		{
			Failure = true;
			return;
		}

		auto target = viewStorage->GetCPUHandle( lock );
		auto from = ppSRVs[i]->Handle;

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		if ( i == 0 )
		{
			base = viewStorage->GetGPUHandle( lock );
		}
	}

	pCommandList->SetGraphicsRootDescriptorTable( ( UINT )slot, base );
}

void CDeviceContext::SetGraphicsRootShaderResources( int slot, int numViews, RefPtr<CShaderResourceView>* ppSRVs )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetGraphicsRootShaderResources(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetGraphicsRootShaderResources(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	for ( int i = 0; i < numViews; ++i )
	{
		auto lock = viewStorage->Lock();
		if ( lock == -1 )
		{
			Failure = true;
			return;
		}

		auto target = viewStorage->GetCPUHandle( lock );
		auto from = ppSRVs[i]->Handle;

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		if ( i == 0 )
		{
			base = viewStorage->GetGPUHandle( lock );
		}
	}

	pCommandList->SetGraphicsRootDescriptorTable( ( UINT )slot, base );
}

void CDeviceContext::SetComputeRootShaderResources( int slot, RefPtr<CShaderResourceView>& pSRV )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootShaderResources(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootShaderResources(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	auto lock = viewStorage->Lock();
	if ( lock != -1 )
	{
		auto target = viewStorage->GetCPUHandle( lock );
		auto from = pSRV->Handle;
		base = viewStorage->GetGPUHandle( lock );

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
		pCommandList->SetComputeRootDescriptorTable( ( UINT )slot, base );
	}
	else
	{
		Failure = true;
	}
}

void CDeviceContext::SetComputeRootShaderResources( int slot, int numViews, CShaderResourceView* const* ppSRVs )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootShaderResources(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootShaderResources(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	for ( int i = 0; i < numViews; ++i )
	{
		auto lock = viewStorage->Lock();
		if ( lock == -1 )
		{
			Failure = true;
			return;
		}

		auto target = viewStorage->GetCPUHandle( lock );
		auto from = ppSRVs[i]->Handle;

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		if ( i == 0 )
		{
			base = viewStorage->GetGPUHandle( lock );
		}
	}

	pCommandList->SetComputeRootDescriptorTable( ( UINT )slot, base );
}

void CDeviceContext::SetComputeRootShaderResources( int slot, int numViews, RefPtr<CShaderResourceView>* ppSRVs )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootShaderResources(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootShaderResources(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	for ( int i = 0; i < numViews; ++i )
	{
		auto lock = viewStorage->Lock();
		if ( lock == -1 )
		{
			Failure = true;
			return;
		}

		auto target = viewStorage->GetCPUHandle( lock );
		auto from = ppSRVs[i]->Handle;

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		if ( i == 0 )
		{
			base = viewStorage->GetGPUHandle( lock );
		}
	}

	pCommandList->SetComputeRootDescriptorTable( ( UINT )slot, base );
}

void CDeviceContext::SetComputeRootUnorderedAccessViews( int slot, RefPtr<CUnorderedAccessView>& pUAV )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootUnorderedAccessViews(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootUnorderedAccessViews(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	auto lock = viewStorage->Lock();
	if ( lock != -1 )
	{
		auto target = viewStorage->GetCPUHandle( lock );
		auto from = pUAV->Handle;
		base = viewStorage->GetGPUHandle( lock );

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
		pCommandList->SetComputeRootDescriptorTable( ( UINT )slot, base );
	}
	else
	{
		Failure = true;
	}
}

void CDeviceContext::SetComputeRootUnorderedAccessViews( int slot, int numViews, CUnorderedAccessView* const* ppUAVs )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootUnorderedAccessViews(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootUnorderedAccessViews(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	for ( int i = 0; i < numViews; ++i )
	{
		auto lock = viewStorage->Lock();
		if ( lock == -1 )
		{
			Failure = true;
			return;
		}
		auto target = viewStorage->GetCPUHandle( lock );
		auto from = ppUAVs[i]->Handle;

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		if ( i == 0 )
		{
			base = viewStorage->GetGPUHandle( lock );
		}
	}

	pCommandList->SetComputeRootDescriptorTable( ( UINT )slot, base );
}

void CDeviceContext::SetComputeRootUnorderedAccessViews( int slot, int numViews, RefPtr<CUnorderedAccessView>* ppUAVs )
{
#if defined( _DEBUG )
	if ( !viewStorage )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootUnorderedAccessViews(): viewStorage is Null." );
	}

	if ( slot < 0 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.SetComputeRootUnorderedAccessViews(): slot < 0." );
	}
#endif

	D3D12_GPU_DESCRIPTOR_HANDLE base;

	for ( int i = 0; i < numViews; ++i )
	{
		auto lock = viewStorage->Lock();
		if ( lock == -1 )
		{
			Failure = true;
			return;
		}
		auto target = viewStorage->GetCPUHandle( lock );
		auto from = ppUAVs[i]->Handle;

		pDevice->CopyDescriptorsSimple( 1, target, from, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		if ( i == 0 )
		{
			base = viewStorage->GetGPUHandle( lock );
		}
	}

	pCommandList->SetComputeRootDescriptorTable( ( UINT )slot, base );
}

void CDeviceContext::DispatchShaderInfo( int numStructs, const tag_ShaderInfo* pShaderInfos )
{
	int frameIndex = GlobalVar.frameIndex;

	auto pBlock = ( tag_ShaderInfo* )uiShaderInfos[frameIndex]->pBlock;
	memcpy( pBlock + uiShaderInfoOffset, pShaderInfos, sizeof( tag_ShaderInfo ) * numStructs );

#if defined( _DEBUG )
	if ( auto slot = Slot["ShaderInfo"]; slot == -1 )
	{
		throw new Exception( "SC.Game.Details.CDeviceContext.DispatchShaderInfo(): Slot of \"ShaderInfo\" is -1." );
	}
#endif

	pCommandList->SetGraphicsRootShaderResourceView( ( UINT )Slot["ShaderInfo"], uiShaderInfos[frameIndex]->VirtualAddress + sizeof( tag_ShaderInfo ) * uiShaderInfoOffset );
	uiShaderInfoOffset += numStructs;

	pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	pCommandList->DrawInstanced( 4, ( UINT )numStructs, 0, 0 );
}

int CDeviceContext::Slot_get( const string_view& param0 )
{
	if ( auto it = slotMap.find( param0 ); it != slotMap.end() )
	{
		return it->second;
	}
	else
	{
		return -1;
	}
}