// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXImmediateContext.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXCommandQueue.h"

DirectXImmediateContext::DirectXImmediateContext(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, D3D12_COMMAND_LIST_TYPE type) : Super(deviceBundle, type)
	, commandQueue(commandQueue)
{

}

DirectXImmediateContext::~DirectXImmediateContext()
{

}

void DirectXImmediateContext::EndDraw()
{
	Super::EndDraw();

	commandQueue->ExecuteCommandLists(this);
	commandQueue->AddPendingReference(this);
}

void DirectXImmediateContext::AddPendingReference(Object* pendingReference)
{
	pendingReferences.emplace_back(pendingReference);
}

void DirectXImmediateContext::AddPendingReference(IUnknown* pendingReference)
{
	class PendingUnknownCapture : virtual public Object
	{
	public:
		using Super = Object;

	private:
		TComPtr<IUnknown> unknown;

	public:
		PendingUnknownCapture(IUnknown* inUnknown) : Super()
			, unknown(inUnknown)
		{

		}
	};

	pendingReferences.emplace_back(NewObject<PendingUnknownCapture>(pendingReference));
}