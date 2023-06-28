// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:IOContext;

export import Core.Std;
export import Core.System;

export class CORE_API IOContext
{
	IOContext(const IOContext&) = delete;

private:
	void* Impl;
	std::atomic<size_t> Workers = 0;

public:
	IOContext();
	~IOContext() noexcept;

	size_t Poll();
	size_t PollOne();

	size_t Run();
	void Stop();

	void BindHandle(void* NativeHandle);
	bool Post(std::function<void(size_t, int32)> InWork);
};