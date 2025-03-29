// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Console.h"
#include "GC/GC.h"
#include "Threading/Tasks/Task.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Platform/PlatformProcess.h"
#include <set>
#include <chrono>

namespace Ayla
{
	std::chrono::seconds GC::TimeSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(29950));
	int32 GC::s_Interlocked;
	int64 GC::s_Version;
	std::thread::id GC::s_ThreadId;
	bool GC::s_DuringFinalize;

	void GC::TrapInitialize()
	{
		if (PlatformAtomics::InterlockedCompareExchange(&s_Interlocked, 1, 0) == 0)
		{
			std::thread([]()
			{
				s_ThreadId = std::this_thread::get_id();
				while (true)
				{
					std::this_thread::sleep_for(GC::TimeSeconds);
					InternalCollect();
				}
			}).detach();
		}
	}

	void GC::SuppressFinalize(const RPtr<Object>& target)
	{
		target->m_FinalizeSuppressed = true;
	}

	void GC::AddRef(Object* target)
	{
		// AddRef must always increment the reference count under the assumption that the Object is alive.
		// Increasing the reference count when it was 0 is a logical error.
		// Therefore, this operation cannot fail.
		check(target != nullptr);
		auto& mark = Object::s_RootCollection.GetMark(target);
		PlatformAtomics::InterlockedIncrement(&mark.Refs);
	}

	void GC::Release(Object* target)
	{
		// If the Release function finalizes an object, it is always at the point when the reference count reaches 0.
		// Therefore, releasing an object with a reference count of 0 multiple times is a logical error.
		check(target != nullptr);
		auto& mark = Object::s_RootCollection.GetMark(target);
		PlatformAtomics::InterlockedDecrement(&mark.Refs);
	}

	void GC::InternalCollect()
	{
		struct ScopedTimer
		{
			PerformanceTimer m_Timer;
			double* m_Output;

			ScopedTimer(double& output)
			{
				m_Timer.Start();
				m_Output = &output;
			}

			~ScopedTimer() noexcept
			{
				m_Timer.Stop();
				*m_Output = m_Timer.GetElapsed().GetTotalSeconds();
			}
		};

		check(s_ThreadId == std::this_thread::get_id());

		// Since FinalizeObject is processed on a different thread,
		// vector memory allocation is done individually for each call.
		static std::vector<Object*> finalizedObjects;
		static std::vector<Object*> gatherQueue;
		static std::vector<Object*> gatherQueue_;

		++s_Version;
		finalizedObjects.clear();
		gatherQueue.clear();
		gatherQueue_.clear();

		auto& self_ = Object::s_RootCollection;
		auto lock = std::unique_lock(self_.m_Mutex);

		finalizedObjects.resize(self_.m_Roots.size());

		double criticalSectionSecs;
		double markObjectsSecs;
		double unmarkPropertyObjectsSecs;
		double finalizeQueueSecs;

		{
			ScopedTimer criticalSection(criticalSectionSecs);

			{
				ScopedTimer markObjects(markObjectsSecs);

				// STEP 1. Mark objects that are not roots.
				for (size_t i = 0; i < self_.m_Roots.size(); ++i)
				{
					auto& mark = self_.m_Roots[i];
					if (mark.Refs == 0 && mark.Ptr != nullptr)
					{
						mark.Version = s_Version;
					}
				}
			}

			{
				ScopedTimer unmarkPropertyObjects(unmarkPropertyObjectsSecs);

				// STEP 2. Unmark objects that are not roots but are referenced by root objects.
				for (size_t i = 0; i < self_.m_Roots.size(); ++i)
				{
					auto& mark = self_.m_Roots[i];
					if (mark.Ptr != nullptr && mark.Refs > 0)
					{
						gatherQueue.emplace_back(mark.Ptr);
						while (gatherQueue.size() != 0)
						{
							for (auto& object : gatherQueue)
							{
								for (auto& ptr : object->m_PPtrCollection.m_PPtrMembers)
								{
									if (ptr->m_Object != nullptr)
									{
										auto& innerMark = self_.GetMark(ptr->m_Object);
										if (innerMark.Version != 0)
										{
											innerMark.Version = 0;
											gatherQueue_.emplace_back(ptr->m_Object);
										}
									}
								}
							}

							std::swap(gatherQueue, gatherQueue_);
							gatherQueue_.clear();
						}

						gatherQueue.clear();
					}
				}
			}

			{
				ScopedTimer finalizeQueue(finalizeQueueSecs);

				// STEP 3. Add marked objects to the Finalize Queue.
				size_t index = 0;
				for (size_t i = 0; i < self_.m_Roots.size(); ++i)
				{
					auto& mark = self_.m_Roots[i];
					if (mark.Refs == 0 && mark.Version == s_Version && mark.Ptr != nullptr)
					{
						finalizedObjects[index++] = self_.FinalizeObject(mark);
					}
				}

				finalizedObjects.resize(index);
			}
		}

		lock.unlock();

		double finalizeSecs;

		{
			ScopedTimer finalize(finalizeSecs);
			s_DuringFinalize = true;

			size_t c = std::thread::hardware_concurrency();
			size_t items = (finalizedObjects.size() - 1) / c + 1;
			std::vector<Task<>> tasks(c);
			for (size_t i = 0; i < c; ++i)
			{
				size_t s = items * i;
				size_t e = std::min(s + items, finalizedObjects.size());
				tasks[i] = Task<>::Run([&, s = s, e = e]()
				{
					for (size_t j = s; j < e; ++j)
					{
						auto* object = finalizedObjects[j];
						if (object->m_FinalizeSuppressed == false)
						{
							object->m_FinalizeSuppressed = true;
							object->Finalize();
						}

						delete object;
					}
				});
			}

			Task<>::WhenAll(tasks).GetResult();
			s_DuringFinalize = false;
		}

		finalizedObjects.clear();

		PlatformProcess::OutputDebugString(String::Format(TEXT("GC::InternalCollect() called. CriticalSection: {:.2f} secs (MarkObjects: {:.2f} secs, UnmarkPropertyObjects: {:.2f} secs, FinalizeQueue: {:.2f} secs), Finalize: {:.2f} secs\n"), criticalSectionSecs, markObjectsSecs, unmarkPropertyObjectsSecs, finalizeQueueSecs, finalizeSecs));
	}
}