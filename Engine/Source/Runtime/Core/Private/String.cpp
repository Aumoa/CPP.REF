// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "String_.h"
#include "Platform/PlatformLocalization.h"
#include <unordered_map>

namespace Ayla
{
	String::String(std::string_view str)
		: String(PlatformLocalization::FromCodepage(str))
	{
	}

	String::String(const char* buf, size_t len)
		: String(PlatformLocalization::FromCodepage(std::string_view(buf, len)))
	{
	}

	String& String::AllocateAssign(const char_t* buf, size_t len)
	{
		auto& ptr = m_Buf.template emplace<1>(std::make_shared<wchar_t[]>(len + 1));
		memcpy(ptr.get(), buf, sizeof(wchar_t) * len);
		ptr.get()[len] = 0;
		m_Len = len;
		m_bNullTerminate = true;
		return *this;
	}

	String& String::AllocateAssign(const char16_t* buf, size_t len)
	{
		auto& ptr = m_Buf.template emplace<1>(std::make_shared<wchar_t[]>(len + 1));
		if constexpr (sizeof(wchar_t) == 2)
		{
			memcpy(ptr.get(), buf, sizeof(wchar_t) * len);
		}
		else
		{
			for (size_t i = 0; i < len; ++i)
			{
				ptr.get()[i] = static_cast<wchar_t>(buf[i]);
			}
		}
		ptr.get()[len] = 0;
		m_Len = len;
		m_bNullTerminate = true;
		return *this;
	}

	String String::FromLiteral(std::string_view str)
	{
		static thread_local std::unordered_map<const char*, String> views;
		if (auto it = views.find(str.data()); it != views.end())
		{
			return it->second;
		}
		else
		{
			String cv = String(str);
			views.emplace(str.data(), cv);
			return cv;
		}
	}

	[[nodiscard]] String String::FromCodepage(std::string_view str, int32 codepage)
	{
		return String(PlatformLocalization::FromCodepage(str, codepage));
	}

	[[nodiscard]] std::string String::AsCodepage(int32 codepage) const
	{
		return PlatformLocalization::ToCodepage((std::wstring_view)*this, codepage);
	}
}