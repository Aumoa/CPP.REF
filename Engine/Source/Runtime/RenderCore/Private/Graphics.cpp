// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Graphics.h"
#include "GenericApplication.h"
#include "CommandBuffer.h"
#include "Buffer.h"

namespace Ayla
{
    Graphics::Graphics()
    {
    }

    Graphics::~Graphics() noexcept
    {
    }

    SharedPtr<CommandBuffer> Graphics::GetPooledCommandBuffer()
    {
        std::unique_lock lock(m_PooledCommandBufferMutex);
        if (m_PooledCommandBuffers.empty())
        {
            lock.unlock();
            return CreateCommandBuffer();
        }
        else
        {
			auto it = m_PooledCommandBuffers.begin() + (m_PooledCommandBuffers.size() - 1);
            auto cmd = std::move(*it);
            m_PooledCommandBuffers.erase(it);
            return cmd;
        }
    }

    void Graphics::ReleasePooledCommandBuffer(SharedPtr<CommandBuffer> cmd)
    {
        std::unique_lock lock(m_PooledCommandBufferMutex);
        m_PooledCommandBuffers.emplace_back(std::move(cmd));
    }

    void Graphics::BeginRenderFrame()
    {
    }

    void Graphics::EndRenderFrame()
    {
        ++m_FrameNumber;
    }
}