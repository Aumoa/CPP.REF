// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/LifetimeTestObject.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/TypeCollector.h"
#include "Marshal/ManagedCallBoundary.h"
#include "ScriptingBackend/ScriptingBackend.h"

ACLASS__IMPL_CLASS_REGISTER(Ayla, LifetimeTestObject);

namespace Ayla
{
	std::atomic<int32> LifetimeTestObject::s_ConstructedCount = 0;
	std::atomic<int32> LifetimeTestObject::s_DestroyedCount = 0;

	LifetimeTestObject::LifetimeTestObject()
	{
		++s_ConstructedCount;
	}

	LifetimeTestObject::~LifetimeTestObject() noexcept
	{
		++s_DestroyedCount;
	}

	void LifetimeTestObject::ResetCounters()
	{
		s_ConstructedCount = 0;
		s_DestroyedCount = 0;
	}

	int32 LifetimeTestObject::GetConstructedCount()
	{
		return s_ConstructedCount;
	}

	int32 LifetimeTestObject::GetDestroyedCount()
	{
		return s_DestroyedCount;
	}

	ManagedTypeWrapper LifetimeTestObject::GetManagedType()
	{
		static ManagedTypeWrapper s_Type = []()
		{
			using signature_t = NativeCallStatus(*)(void**);
			auto function = reinterpret_cast<signature_t>(ScriptingBackend::Get().GetFunctionPointer(
				"AylaUnitTests.Script",
				"Ayla.Tests.ManagedLifetimeObject",
				"GetScriptType__Invoke"));
			void* scriptTypeGetter = nullptr;
			ManagedCallBoundary::ThrowIfFailed(function(&scriptTypeGetter));

			return ManagedTypeWrapper
			{
				.NativeType = TypeCollector::FindType(typeid(LifetimeTestObject)),
				.ScriptTypeGetter = scriptTypeGetter
			};
		}();

		return s_Type;
	}
}
