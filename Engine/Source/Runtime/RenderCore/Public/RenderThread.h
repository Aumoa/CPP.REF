// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline RenderCore
{
	class SRenderContext;

	class RENDERCORE_API SRenderThread : implements(SObject)
	{
		GENERATED_BODY(SRenderThread, AutoRegistration());

	public:
		using RenderThreadWork = std::function<void(SRenderContext*)>;

	private:
		struct Work
		{
			WeakPtr<SObject> Holder;
			RenderThreadWork Body;
		};

	private:
		static SRenderThread* sInstance;

		Thread* _thread = nullptr;
		bool _running = false;

		Spinlock _lock;
		SpinlockConditionVariable _invoke;
		SpinlockConditionVariable _dequeued;
		std::exception_ptr _exception;

		std::vector<Work> _queuedWorks;
		SRenderContext* _rcontext;

	public:
		SRenderThread();
		virtual ~SRenderThread() noexcept override;

		void EnqueueRenderThreadWork(SObject* object, RenderThreadWork work);
		void ExecuteWorks(SRenderContext* InRenderContext, RenderThreadWork InCompletionWork);
		void Shutdown();
		static bool InRenderThread();

	public:
		static SRenderThread* Get() { return sInstance; }

	private:
		void Run();
	};
}