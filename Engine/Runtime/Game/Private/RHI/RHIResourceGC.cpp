// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIResourceGC.h"

#include "Engine.h"
#include "RHICommon.h"
#include "Logging/LogMacros.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/IRHIImmediateCommandList.h"
#include "RHI/IRHIFence.h"

using namespace std;

RHIResourceGC::RHIResourceGC() : Super()
	, issued(0)
	, lastCollected(0)
{
	fence = GEngine.DeviceBundle->CreateFence();
}

RHIResourceGC::~RHIResourceGC()
{

}

uint64 RHIResourceGC::IssueNumber()
{
	ScopedLock(locker);

	return ++issued;
}

void RHIResourceGC::AddPendingResource(TRefPtr<Object> resource, uint64 number)
{
	if (number == 0)
	{
		SE_LOG(LogRHI, Error, L"Issue number cannot be a zero.");
		return;
	}

	ScopedLock(locker);

	uint64 issued_index = number - lastCollected - 1;
	if (issuedContainers.size() <= (size_t)issued_index)
	{
		issuedContainers.resize((size_t)issued_index + 1);
	}

	Container& cnt = issuedContainers[issued_index];
	cnt.emplace_back(move(resource));
}

void RHIResourceGC::SignalNumber(uint64 number)
{
	IRHIImmediateCommandList* immediateCommandList = GEngine.DeviceBundle->GetImmediateCommandList();
	fence->Signal(immediateCommandList, issued);
}

void RHIResourceGC::Collect()
{
	uint64 fenceValue = fence->GetCompletedValue();
	if (fenceValue > lastCollected)
	{
		size_t collect_count = fenceValue - lastCollected;
		auto beg = issuedContainers.begin();
		issuedContainers.erase(beg, beg + collect_count);
		lastCollected = fenceValue;
	}
}