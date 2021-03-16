// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceContext.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

using namespace std;

DirectXDeviceContext::DirectXDeviceContext(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type) : Super(deviceBundle)
	, device(deviceBundle->GetDevice())
	, commandType(type)

	, bHasBegunDraw(false)
{
	HR(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));
}

DirectXDeviceContext::~DirectXDeviceContext()
{

}

void DirectXDeviceContext::BeginDraw(ID3D12PipelineState* initialPipeline)
{
	HR(commandAllocator->Reset());
	if (commandList.IsValid)
	{
		HR(commandList->Reset(commandAllocator.Get(), initialPipeline));
	}
	else
	{
		HR(device->CreateCommandList(0, commandType, commandAllocator.Get(), initialPipeline, IID_PPV_ARGS(&commandList)));
	}

	bHasBegunDraw = true;
}

void DirectXDeviceContext::EndDraw()
{
	HR(commandList->Close());
	bHasBegunDraw = false;
}

ID3D12GraphicsCommandList4* DirectXDeviceContext::GetCommandList() const
{
	return commandList.Get();
}

D3D12_COMMAND_LIST_TYPE DirectXDeviceContext::GetCommandListType() const
{
	return commandType;
}

void DirectXDeviceContext::AddPendingReference(TRefPtr<Object>&& pendingReference)
{
	pendingReferences.emplace_back(move(pendingReference));
}

void DirectXDeviceContext::AddPendingReference(TComPtr<IUnknown>&& pendingReference)
{
	class PendingUnknownCapture : virtual public Object
	{
	public:
		using Super = Object;

	private:
		TComPtr<IUnknown> unknown;

	public:
		PendingUnknownCapture(TComPtr<IUnknown>& inUnknown) : Super()
			, unknown(move(inUnknown))
		{

		}
	};

	pendingReferences.emplace_back(NewObject<PendingUnknownCapture>(pendingReference));
}

void DirectXDeviceContext::SwapCommandAllocator(TComPtr<ID3D12CommandAllocator>& swapTarget)
{
	commandAllocator.Swap(swapTarget);
}