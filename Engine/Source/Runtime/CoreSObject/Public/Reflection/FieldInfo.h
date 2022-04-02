// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "MemberInfo.h"
#include "FieldInfoMetadataGenerator.h"
#include "Type.h"
#include <span>

namespace libty::inline Core::Reflection
{
	/// <summary>
	/// Discovers the attributes of a field and provides access to field metadata.
	/// </summary>
	class CORESOBJECT_API SFieldInfo : public SMemberInfo
	{
		GENERATED_BODY(SFieldInfo);
		friend class libty::Core::GarbageCollector;

	public:
		using MetadataGenerator = libty::Core::Reflection::FieldInfoMetadataGenerator;

	private:
		MetadataGenerator _meta;

	public:
		SFieldInfo(MetadataGenerator&& generator);

		/// <summary>
		/// Gets the name of current field.
		/// </summary>
		virtual std::wstring GetName() override;

		/// <summary>
		/// Gets a value indicating whether the field is readonly.
		/// </summary>
		virtual bool IsReadOnly();

		/// <summary>
		/// Gets a value indicating whether the field is static.
		/// </summary>
		virtual bool IsStatic();

		/// <summary>
		/// Sets the value of the field for the given object to the given value.
		/// </summary>
		/// <param name="obj"> The object whose field value will be set. </param>
		/// <param name="value"> The value to assign to the field. </param>
		virtual void SetValue(SObject* obj, SObject* value);

		/// <summary>
		/// Returns the value of a field supported by a given object.
		/// </summary>
		/// <param name="obj"> The object whose field value will be returned. </param>
		virtual SObject* GetValue(SObject* obj);

		/// <summary>
		/// Returns the attributes of a field declared by a given object.
		/// </summary>
		virtual std::span<SFieldAttribute* const> GetAttributes();

		/// <summary>
		/// Returns the attribute of a field declared by a given object.
		/// </summary>
		virtual SFieldAttribute* GetAttribute(SType* attributeType);

		/// <summary>
		/// Returns the attribute of a field declared by a given object.
		/// </summary>
		template<std::derived_from<SFieldAttribute> T>
		T* GetAttribute()
		{
			return Cast<T>(GetAttribute(SType::template TypeOf<T>()));
		}

		/// <summary>
		/// Gets the type of this field object.
		/// </summary>
		virtual SType* GetFieldType();
	};
}