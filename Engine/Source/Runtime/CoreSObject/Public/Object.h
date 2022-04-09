// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "gcnew.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/TypeInfoMetadataGenerator.h"
#include "Reflection/FieldInfoMetadataGenerator.h"
#include "Reflection/MethodInfoMetadataGenerator.h"
#include "GC/Referencer.h"
#include <string>
#include <string_view>
#include <atomic>
#include <functional>

namespace libty::inline Core
{
	class SType;

	/// <summary>
	/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
	/// </summary>
	class CORESOBJECT_API SObject
	{
		friend struct libty::Core::Reflection::TypeInfoMetadataGenerator;

	public:
		using This = SObject;
		using Super = void;
		using IsClass = SObject;

		inline static constexpr std::wstring_view FriendlyName = L"SObject";

	private:
		static SType StaticClass;

	public:
		static inline SType* TypeId = &StaticClass;
		virtual SType* GetType() const
		{
			return TypeId;
		}
	
	private:
		template<size_t _Line>
		static consteval size_t REFLECTION_FunctionChain()
		{
			return REFLECTION_FunctionChain<_Line - 1>();
		}

		template<size_t _Line> requires (_Line == __LINE__)
		static consteval size_t REFLECTION_FunctionChain()
		{
			return -1;
		}

		template<size_t _Line>
		static consteval size_t REFLECTION_PropertyChain()
		{
			return REFLECTION_PropertyChain<_Line - 1>();
		}

		template<size_t _Line> requires (_Line == __LINE__)
		static consteval size_t REFLECTION_PropertyChain()
		{
			return -1;
		}

		template<size_t>
		static void REFLECTION_GetFunctionPointer(void*);
		template<size_t>
		static consteval void REFLECTION_GetFunctionName(void*);
		template<size_t>
		static void REFLECTION_GetPropertyPointer(void*);

	private:
		template<class T>
		friend class SharedPtr;
		template<class T>
		friend class WeakPtr;
		friend class GarbageCollector;
		friend class ObjectHashTable;
		friend class libty::Core::Reflection::GCNewBinder;
		friend struct IDisposable;

	private:
		uint8 bMarkAtGC : 1 = false;
		uint8 bHasFinalizer : 1 = false;
		size_t InternalIndex = -1;
		Referencer* ReferencePtr = nullptr;

	#if !SHIPPING
		std::wstring CachedTypeName;
		SObject* GC_ContainsOwner = nullptr;
	#endif

	public:
		SObject();
		SObject(SObject&& Rhs) noexcept;
		virtual ~SObject() noexcept;

		SFUNCTION(AddToRoot)
		void AddToRoot();
		SFUNCTION(RemoveFromRoot)
		void RemoveFromRoot();
		std::function<bool()> GetHolder();

	private:
		SObject(const SObject&) = delete;

	public:
		SFUNCTION(ToString)
		virtual std::wstring ToString();

	protected:
		virtual void PostConstruction();
		virtual void Dispose(bool bDisposing);

	public:
		void* operator new(size_t);
		void operator delete(void*);
	};

	template<class T>
	T* libty::Core::Reflection::GCNewBinder::operator << (T* Ptr)
	{
		((SObject*)Ptr)->PostConstruction();
		return Ptr;
	}
}

#include "ObjectCasts.h"