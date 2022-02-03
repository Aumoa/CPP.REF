// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

class SObject;
class Type;

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
			((SObject*)Ptr)->PostConstruction();
			return Ptr;
		}
	};
}