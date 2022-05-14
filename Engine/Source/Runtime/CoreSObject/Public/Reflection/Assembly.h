// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <set>

namespace libty::inline Core::Reflection
{
	class CORESOBJECT_API SAssembly : implements(SObject)
	{
		GENERATED_BODY(SAssembly);
		friend class libty::Core::SType;

	private:
		String _assemblyName;
		std::set<SType*>* volatile _types;

	public:
		SAssembly(String assemblyName);
		virtual ~SAssembly() noexcept override;

		/// <summary>
		/// Gets assembly name.
		/// </summary>
		virtual String GetName();

		/// <summary>
		/// Gets all types declared in current assembly.
		/// </summary>
		virtual std::set<SType*> GetTypes();

	private:
		void AddType(SType* type);
	};

	#define DEFINE_ASSEMBLY_INFO \
namespace libty::Generated::Assemblies \
{ \
	::libty::Core::Reflection::SAssembly SE_ASSEMBLY_INFO(SE_ASSEMBLY_NAME); \
}
}