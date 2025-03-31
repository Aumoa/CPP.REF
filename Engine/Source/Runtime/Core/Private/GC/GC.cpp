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

	std::chrono::seconds GC::TimeSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(29950));
	int32 GC::s_Interlocked;
	int64 GC::s_Version;
	std::thread::id GC::s_ThreadId;
	int32 GC::s_DuringFinalize;
	std::mutex GC::s_NotifyMtx;
	std::condition_variable GC::s_CompleteToFinalize;

	void GC::SuppressFinalize(const RPtr<Object>& target)
	{
		target->m_FinalizeSuppressed = true;
	}

	void GC::Collect(int32 generation)
	{
		InternalCollect(generation, false);
	}

	void GC::WaitForCompleteToFinalize()
	{
		auto lock = std::unique_lock(s_NotifyMtx);
		while (s_DuringFinalize > 0)
		{
			s_CompleteToFinalize.wait(lock);
		}
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

	std::unique_lock<std::mutex> GC::GetLock()
	{
		return std::unique_lock(Object::s_RootCollection.m_Mutex);
	}

	void GC::InternalCollect(int32 generation, bool nolock)
	{
		std::vector<Object*> finalizedObjects;

		// Since FinalizeObject is processed on a different thread,
		// vector memory allocation is done individually for each call.
		static std::vector<Object*> gatherQueue;
		static std::vector<Object*> gatherQueue_;

		gatherQueue.clear();
		gatherQueue_.clear();

		auto& self_ = Object::s_RootCollection;
		if (nolock == false)
		{
			self_.m_Mutex.lock();
		}

		if (generation == 0 && self_.m_InstanceIndexPool[1].size() < Object::RootCollection::G1Size)
		{
			InternalCollect(1, true);
		}

		double criticalSectionSecs;
		double markObjectsSecs;
		double unmarkPropertyObjectsSecs;
		double finalizeQueueSecs;

		size_t begin, end;
		switch (generation)
		{
		case 0:
			begin = 0;
			end = Object::RootCollection::G1Size;
			break;
		case 1:
			begin = Object::RootCollection::G1Size;
			end = begin + Object::RootCollection::G2Size;
			break;
		default:
			begin = 0;
			end = self_.m_Roots.size();
			break;
		}

		++s_Version;

		{
			ScopedTimer criticalSection(criticalSectionSecs);

			{
				ScopedTimer markObjects(markObjectsSecs);

				// STEP 1. Mark objects that are not roots.
				for (size_t i = begin; i < end; ++i)
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
				for (size_t i = begin; i < end; ++i)
				{
					auto& mark = self_.m_Roots[i];
					if (mark.Ptr != nullptr)
					{
						if (mark.Refs == 0 && mark.Version == s_Version)
						{
							finalizedObjects.emplace_back(self_.FinalizeObject(mark));
						}
						else if (generation == 0 || generation == 1)
						{
							int32 ngen = generation + 1;
							auto& pool = self_.m_InstanceIndexPool[generation];
							pool.emplace_back(i);
							auto& npool = self_.m_InstanceIndexPool[ngen];
							auto rb = npool.rbegin();
							int64 index;
							Object::RootMark* nmark;
							if (rb == npool.rend())
							{
								check(ngen == 2);
								index = self_.m_Roots.size();
								nmark = &self_.m_Roots.emplace_back(Object::RootMark{});
							}
							else
							{
								index = *rb;
								npool.erase((rb + 1).base());
								nmark = &self_.m_Roots[index];
							}

							// Do NOT use 'mark' variable after this scope.

							std::swap(*nmark, self_.m_Roots[i]);
							nmark->Ptr->m_InstanceIndex = index;
						}
					}
				}
			}
		}

		DoFinalize(generation, std::move(finalizedObjects), criticalSectionSecs, markObjectsSecs, unmarkPropertyObjectsSecs, finalizeQueueSecs);

		if (nolock == false)
		{
			self_.m_Mutex.unlock();
		}
	}

	void GC::DoFinalize(int32 generation, std::vector<Object*> finalizedObjects, double criticalSectionSecs, double markObjectsSecs, double unmarkPropertyObjectsSecs, double finalizeQueueSecs)
	{
		size_t lo = Object::s_LiveObjects - finalizedObjects.size();
		auto& r = Object::s_RootCollection;
		size_t g1 = r.G1Size - r.m_InstanceIndexPool[0].size();
		size_t g2 = r.G2Size - r.m_InstanceIndexPool[1].size();
		size_t g3 = r.m_Roots.size() - r.G1Size - r.G2Size - r.m_InstanceIndexPool[2].size();
		PlatformAtomics::InterlockedIncrement(&s_DuringFinalize);
		co_await Task<>::Yield();

		double finalizeSecs;
		{
			ScopedTimer finalize(finalizeSecs);
			for (auto& object : finalizedObjects)
			{
				if (object->m_FinalizeSuppressed == false)
				{
					object->m_FinalizeSuppressed = true;
					object->Finalize();
				}
				
				delete object;
			}
		}

		PlatformProcess::OutputDebugString(String::Format(TEXT("GC::InternalCollect({})) called. NumCollect: {}, Live: {}(Gen: {}, {}, {}), CriticalSection: {:.2f} secs (Mark: {:.2f} secs, Unmark: {:.2f} secs, FinalizeQueue: {:.2f} secs), Finalize: {:.2f} secs\n"), generation, finalizedObjects.size(), lo, g1, g2, g3, criticalSectionSecs, markObjectsSecs, unmarkPropertyObjectsSecs, finalizeQueueSecs, finalizeSecs));

		auto lock = std::unique_lock(s_NotifyMtx);
		PlatformAtomics::InterlockedDecrement(&s_DuringFinalize);
		s_CompleteToFinalize.notify_all();
	}
}