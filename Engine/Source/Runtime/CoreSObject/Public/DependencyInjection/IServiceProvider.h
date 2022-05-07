// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/Type.h"
#include "InjectException.h"

namespace libty::inline Core
{
	struct CORESOBJECT_API IServiceProvider : implements(SObject)
	{
		GENERATED_INTERFACE_BODY(IServiceProvider);

	public:
		virtual SObject* GetService(SType* type) = 0;
		virtual SObject* Create(SType * type, std::function<SObject*(IServiceProvider*)> factory = nullptr) = 0;

		inline SObject* GetRequiredService(SType* type)
		{
			SObject* instanced = GetService(type);
			if (instanced == nullptr)
			{
				throw InjectException(EInjectionErrorCode::ServiceNotFound);
			}
			return instanced;
		}

		template<class T>
		inline T* GetService()
		{
			return Cast<T>(GetService(typeof(T)));
		}

		template<class T>
		inline T* GetRequiredService()
		{
			return Cast<T>(GetRequiredService(typeof(T)));
		}
	};
}