// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Process;

export import :Std;
export import :ProcessStartInfo;

export class CORE_API Process
{
private:
	ProcessStartInfo StartInfo;
	void* ProcessHandle = nullptr;

private:
	Process();

public:
	~Process() noexcept;

	const ProcessStartInfo& GetStartInfo() const { return StartInfo; }

	static std::shared_ptr<Process> Start(const ProcessStartInfo& InStartInfo);

private:
	void StartApplication();
};