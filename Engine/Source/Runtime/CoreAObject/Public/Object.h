// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AObject;
class ObjectReference;
template<class T>
class TSharedPtr;
class AType;

#define AYLA_DECLARE_STATIC_CLASS_FUNCTION(Namespace, AssemblyName, ClassName) \
public: \
	static AType* StaticClass();

#define AYLA_DEFINE_STATIC_CLASS_FUNCTION(Namespace, AssemblyName, ClassName) \
AType* A ## ClassName::StaticClass() \
{ \
	return &__Ayla_RuntimeType_ ## Namespace ## _ ## AssemblyName ## _ ## ClassName; \
}

#define AYLA_DECLARE_CLASS_TYPEDEFS(Namespace, AssemblyName, ClassName) \
public: \
	using Super = This; \
	using This = A ## ClassName;

class COREAOBJECT_API AObjectBase
{
protected:
	using This = AObjectBase;

protected:
	AObjectBase()
	{
	}

public:
	virtual ~AObjectBase() noexcept
	{
	}

	static AType* StaticClass() noexcept
	{
		return nullptr;
	}
};

class COREAOBJECT_API AObject : public AObjectBase
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, Object);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, Object);

private:
	struct ObjectInitializer;
	friend struct ReflectionUtility;

private:
	AType* classType = nullptr;
	ObjectReference* referencer = nullptr;

protected:
	AObject(std::in_place_t builtIn);
	AObject();

public:
	virtual ~AObject() noexcept override;

	AType* GetType();

	static AObject* NewObject(AType* classType);
	static void Destroy(AObject* instance);

	template<class T>
	static T* NewObject() requires std::derived_from<T, AObject>
	{
		return (T*)NewObject(T::StaticClass());
	}
};