// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core
{	
	template<class TChar> requires TIsChar<TChar>
	inline size_t String::Strlen(const TChar* text)
	{
		size_t length = 0;
		for (auto item = text; *item != 0; ++item)
		{
			length += 1;
		}
		return length;
	}

	template<TIsRefCore T>
	template<class TChar> requires TIsChar<TChar> && TIsAssignable<String, T>
	inline TRefPtr<T>::TRefPtr(const TChar* text) : This(new String(text))
	{
		ptr->bLockCollecting = false;
	}
}