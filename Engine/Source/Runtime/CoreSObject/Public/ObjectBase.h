// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

class SObject;
class Type;

template<class T>
concept HasSuper = requires { typename T::Super; } && !std::same_as<typename T::Super, void>;

namespace SObjectDetails
{
	class SObjectBase
	{
		virtual Type* GetType() const = 0;
	};

	class GCNewBinder
	{
	public:
		GCNewBinder()
		{
		}

		template<class T>
		T* operator << (T* Ptr)
		{
			Ptr->PostConstruction();
			return Ptr;
		}
	};
}