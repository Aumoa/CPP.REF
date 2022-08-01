// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHICommandList.h"

NullRHICommandList::NullRHICommandList(std::shared_ptr<RHIDevice> device)
	: RHICommandList(std::move(device))
{
}

NullRHICommandList::~NullRHICommandList() noexcept
{
}

void NullRHICommandList::BeginFrame()
{
	check(_begun == false);
	_begun = true;
}

void NullRHICommandList::EndFrame()
{
	check(_begun);
	_begun = false;
}