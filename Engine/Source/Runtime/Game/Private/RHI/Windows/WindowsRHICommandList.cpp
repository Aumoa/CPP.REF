// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHICommandList.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"

std::shared_ptr<RHICommandList> WindowsRHIDevice::CreateCommandList()
{
	return MakeShared<WindowsRHICommandList>(SharedFromThis());
}

WindowsRHICommandList::WindowsRHICommandList(std::shared_ptr<WindowsRHIDevice> device)
	: RHICommandList(device)
{
	auto* d3ddev = device->GetDevice();
	HR(d3ddev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_allocator)));
}

WindowsRHICommandList::~WindowsRHICommandList() noexcept
{
}

void WindowsRHICommandList::BeginFrame()
{
	check(_begun == false);
	
	HR(_allocator->Reset());
	if (_commandList)
	{
		HR(_commandList->Reset(_allocator.Get(), nullptr));
	}
	else
	{
		auto device = GetDevice();
		auto* d3ddev = std::dynamic_pointer_cast<WindowsRHIDevice>(device)->GetDevice();
		HR(d3ddev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _allocator.Get(), nullptr, IID_PPV_ARGS(&_commandList)));
	}

	_begun = true;
}

void WindowsRHICommandList::EndFrame()
{
	check(_begun);

	HR(_commandList->Close());
	_begun = false;
}

#endif