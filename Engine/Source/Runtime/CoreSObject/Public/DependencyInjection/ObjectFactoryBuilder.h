// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IServiceCollection.h"
#include <functional>

namespace libty::inline Core
{
	class SObjectFactory;
	struct IServiceProvider;

	class CORESOBJECT_API SObjectFactoryBuilder : implements(SObject, IServiceCollection)
	{
		GENERATED_BODY(SObjectFactoryBuilder);
		friend class SObjectFactory;

	private:
		SPROPERTY(_factory)
		SObjectFactory* _factory = nullptr;

	private:
		SObjectFactoryBuilder(SObjectFactory* factory);

	public:
		virtual IServiceCollection* AddService(EServiceType type, SType* objectClass, std::function<SObject*(IServiceProvider*)> factory = nullptr) override;
		virtual IServiceProvider* Build() override;
	};
}