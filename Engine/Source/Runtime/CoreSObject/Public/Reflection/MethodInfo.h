// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "MemberInfo.h"
#include "MethodInfoMetadataGenerator.h"
#include "Type.h"

namespace libty::inline Core::Reflection
{
	class CORESOBJECT_API SMethodInfo : public SMemberInfo
	{
		GENERATED_BODY(SMethodInfo);

	public:
		using MetadataGenerator = libty::Core::Reflection::MethodInfoMetadataGenerator;

	private:
		MetadataGenerator _meta;

	public:
		SMethodInfo(MetadataGenerator&& generator);

		/// <summary>
		/// Gets the name of current field.
		/// </summary>
		virtual String GetName() override;

		/// <summary>
		/// Gets a value indicating whether the field is static.
		/// </summary>
		virtual bool IsStatic();

		/// <summary>
		/// Returns the attributes of a field declared by a given object.
		/// </summary>
		virtual std::span<SMethodAttribute* const> GetAttributes();

		/// <summary>
		/// Returns the attribute of a field declared by a given object.
		/// </summary>
		virtual SMethodAttribute* GetAttribute(SType* attributeType);

		/// <summary>
		/// Returns the attribute of a field declared by a given object.
		/// </summary>
		template<std::derived_from<SMethodAttribute> T>
		T* GetAttribute()
		{
			return Cast<T>(GetAttribute(SType::TypeOf<T>()));
		}

		/// <summary>
		/// Gets the return type of this method.
		/// </summary>
		virtual SType* GetReturnType();

		/// <summary>
		/// Get all parameters type of this method.
		/// </summary>
		virtual std::span<SType*> GetParameters();

		/// <summary>
		/// Invokes the method represented by the current instance, using the specified parameters.
		/// </summary>
		/// <param name="self"> The object on which to invoke the method. If a method is static, this argument is ignored. </param>
		/// <param name="args"> An argument list for the invoked method. This is an array of objects with the same number, order, and type as the parameters of the method to be invoked. If there are no parameters, parameters should be zero span. </param>
		/// <returns> An object containing the return value of the invoked method. </returns>
		virtual SObject* Invoke(SObject* self, std::span<SObject* const> args);
	};
}