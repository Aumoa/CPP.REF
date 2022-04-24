// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline RenderCore
{
	class RENDERCORE_API SRenderThread : implements(SObject)
	{
		GENERATED_BODY(SRenderThread);

	private:
		struct Work
		{
			WeakPtr<SObject> Holder;
			std::function<void(IRHIGraphicsCommandList*)> Body;
		};

	private:
		static SRenderThread* sInstance;

		Thread* _thread = nullptr;
		bool _running = false;

		std::mutex _lock;
		std::queue<Work> _queuedWorks;
		std::condition_variable _invoke;
		IRHIGraphicsCommandList* _deviceContext = nullptr;
		std::function<void(IRHIGraphicsCommandList*)> _completion;
		TaskCompletionSource<> _taskCompletionSource;

	private:
		SRenderThread();

	public:
		virtual ~SRenderThread() noexcept override;

		void EnqueueRenderThreadWork(SObject* object, std::function<void(IRHIGraphicsCommandList*)> work);
		Task<> ExecuteWorks(IRHIGraphicsCommandList* InDeviceContext, std::function<void(IRHIGraphicsCommandList*)> InCompletionWork);
		static bool InRenderThread();

	public:
		static SRenderThread* Get() { return sInstance; }

	private:
		void Run();
	};
}