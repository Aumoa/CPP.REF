// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:PerformanceTimer;

export import :Std;
export import :TimeSpan;
export import :DateTime;

export class CORE_API PerformanceTimer
{
public:
	PerformanceTimer() noexcept {}
	PerformanceTimer(std::in_place_t) noexcept { Start(); }
	PerformanceTimer(const PerformanceTimer& Rhs) noexcept;
	PerformanceTimer(PerformanceTimer&& Rhs) noexcept;
	~PerformanceTimer() noexcept {}

	void Start() noexcept;
	void Stop() noexcept;

	void Reset() noexcept;
	void Restart() noexcept;

	bool IsRunning() const noexcept;
	TimeSpan GetElapsed() const noexcept;

	static PerformanceTimer StartNew() noexcept;

public:
	PerformanceTimer& operator =(const PerformanceTimer& Rhs) noexcept;
	PerformanceTimer& operator =(PerformanceTimer&& Rhs) noexcept;

private:
	std::optional<DateTime> StartTime;
	TimeSpan Elapsed;
};