// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

#include "CoreConcepts.h"
#include "TRefPtr.h"

namespace SC::Runtime::Core
{
	class CORE_API String : virtual public Object
	{
	public:
		static wchar_t EmptyBuffer[1];

		using Super = Object;
		using This = String;

	private:
		wchar_t* text_buffer;
		size_t len;
		bool bDynamicBuffer : 1;

	public:
		String();
		~String() override;

		String(const char* text);
		String(const wchar_t* text);
		String(const char* text, size_t len);
		String(const wchar_t* text, size_t len);

		vs_property_get(const wchar_t*, C_Str);
		const wchar_t* C_Str_get() const;
		vs_property_get(size_t, Length);
		size_t Length_get() const;

	private:
		wchar_t* MultiByteToWideChar(const char* multibyte, size_t* len);
		wchar_t* CopyAllocate(const wchar_t* text, size_t len);

		template<class TChar> requires TIsChar<TChar>
		inline static size_t Strlen(const TChar* text);
	};
}

#include "String.inl"