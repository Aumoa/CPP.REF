using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

CCommandQueue::CCommandQueue( CDevice* device, D3D12_COMMAND_LIST_TYPE type ) : Object()
	, type( type )
{
	auto pDevice = device->pDevice.Get();

	// 커맨드 큐 개체를 생성합니다.
	D3D12_COMMAND_QUEUE_DESC queueDesc{ };
	queueDesc.Type = type;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	HR( pDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS( &pCommandQueue ) ) );

	// 펜스 개체를 생성합니다.
	HR( pDevice->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &pFence ) ) );
}

void CCommandQueue::Execute( RefPtr<CDeviceContext>& deviceContext )
{
	if ( !deviceContext->Failure )
	{
		ID3D12CommandList* ppCommandLists[]{ deviceContext->pCommandList.Get() };
		pCommandQueue->ExecuteCommandLists( 1, ppCommandLists );
	}
}

void CCommandQueue::Execute( int numContexts, CDeviceContext* const* ppDeviceContexts )
{
	vector<ID3D12CommandList*> ppCommandLists;

	for ( int i = 0; i < numContexts; ++i )
	{
		if ( !ppDeviceContexts[i]->Failure )
		{
			ppCommandLists.push_back( ppDeviceContexts[i]->pCommandList.Get() );
		}
	}

	if ( ppCommandLists.size() )
	{
		pCommandQueue->ExecuteCommandLists( ( UINT )numContexts, ppCommandLists.data() );
	}
}

uint64 CCommandQueue::Signal()
{
	HR( pCommandQueue->Signal( pFence.Get(), ++LastPending ) );
	return LastPending;
}

void CCommandQueue::Signal( ID3D12Fence* pFence, uint64 fenceValue )
{
	HR( pCommandQueue->Signal( pFence, fenceValue ) );
}

void CCommandQueue::WaitFor( uint64 fenceValue, RefPtr<Threading::Event>& waitingHandle, uint32 limit )
{
	if ( pFence->GetCompletedValue() < fenceValue )
	{
		HR( pFence->SetEventOnCompletion( fenceValue, waitingHandle->Handle ) );
		auto ret = waitingHandle->WaitForSingleObject( limit );

		if ( ret == Threading::AsyncWaitResult::Timeout )
		{
			throw new Exception( "SC.Game.Details.CCommandQueue.WaitFor(): Wait timeout." );
		}
	}
}