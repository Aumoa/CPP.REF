// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/ObjectFactoryBuilder.h"
#include "DependencyInjection/ObjectFactory.h"
#include "Reflection/Type.h"

using namespace ::libty;

SObjectFactoryBuilder::SObjectFactoryBuilder(SObjectFactory* factory)
	: Super()
	, _factory(factory)
{
}

IServiceCollection* SObjectFactoryBuilder::AddService(EServiceType type, SType* objectClass, std::function<SObject*(IServiceProvider*)> factory)
{
	size_t hash = objectClass->GetHashCode();

	if (factory == nullptr && !objectClass->IsConstructible())
	{
		throw InjectException(EInjectionErrorCode::ServiceNotConstructible);
	}

	auto [iter, success] = _factory->_injections.emplace(hash, SObjectFactory::InjectionInfo
	{
		.ServiceType = type,
		.Class = objectClass,
		.Factory = std::move(factory)
	});

	return this;
}

void SObjectFactoryBuilder::SetAsPrimary()
{
	_factory->SetAsPrimary();
}

IServiceProvider* SObjectFactoryBuilder::Build()
{
	SObjectFactory* builded = _factory;
	_factory = nullptr;
	return builded;
}