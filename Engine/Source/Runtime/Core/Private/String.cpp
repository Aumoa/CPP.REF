// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "String_.h"
#include "Platform/PlatformLocalization.h"
#include <unordered_map>

namespace Ayla
{
	String::String(std::string_view Str)
		: String(PlatformLocalization::FromCodepage(Str))
	{
	}

	String::String(const char* Buf, size_t Len)
		: String(PlatformLocalization::FromCodepage(std::string_view(Buf, Len)))
	{
	}

	String& String::AllocateAssign(const char_t* InBuf, size_t InLen)
	{
		auto& Ptr = Buf.emplace<1>(std::make_shared<wchar_t[]>(InLen + 1));
		memcpy(Ptr.get(), InBuf, sizeof(wchar_t) * InLen);
		Ptr.get()[InLen] = 0;
		Len = InLen;
		bNullTerminate = true;
		return *this;
	}

	String String::FromLiteral(std::string_view InStr)
	{
		static thread_local std::unordered_map<const char*, String> Views;
		if (auto It = Views.find(InStr.data()); It != Views.end())
		{
			return It->second;
		}
		else
		{
			String Cv = String(InStr);
			Views.emplace(InStr.data(), Cv);
			return Cv;
		}
	}

	[[nodiscard]] String String::FromCodepage(std::string_view Str, int32 Codepage)
	{
		return String(PlatformLocalization::FromCodepage(Str, Codepage));
	}

	[[nodiscard]] std::string String::AsCodepage(int32 Codepage) const
	{
		return PlatformLocalization::ToCodepage((std::wstring_view)*this, Codepage);
	}
}