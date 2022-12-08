// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <nlohmann/json.hpp>

// Declare custom formats.

inline void from_json(const nlohmann::json& json, String& p)
{
	p = String::FromCodepage((std::string)json, 65001);
}

inline void to_json(nlohmann::json& json, const String& p)
{
	json = p.AsCodepage(65001);
}

inline void from_json(const nlohmann::json& json, IPAddress& p)
{
	String str;
	from_json(json, str);
	bool status = IPAddress::TryParse(str, &p);
	if (status == false)
	{
		throw InvalidCastException(TEXTF("String {} is not a valid IPAddress format.", str));
	}
}

inline void to_json(nlohmann::json& json, const IPAddress& p)
{
	to_json(json, p.ToString());
}



template<class T>
void from_json(const nlohmann::json& json, T& p) requires
	requires { { T::from_json(json, p) }; }
{
	T::from_json(json, p);
}

template<class T>
void to_json(nlohmann::json& json, const T& p) requires
	requires { { std::declval<const T>().to_json(json) }; }
{
	p.to_json(json);
}

namespace JsonUtility
{
	template<class T>
	struct is_optional
	{
		static constexpr bool value = false;
	};

	template<class T>
	struct is_optional<std::optional<T>>
	{
		static constexpr bool value = true;
	};

	template<class T>
	constexpr bool is_optional_v = is_optional<T>::value;

	template<class T, class BeginIt, class EndIt>
	void from_json_member(T& body, const BeginIt& it, const EndIt& end)
	{
		if constexpr (is_optional_v<T>)
		{
			if (it != end && it->is_null() == false)
			{
				body = *it;
			}
			else
			{
				body = std::nullopt;
			}
		}
		else
		{
			body = *it;
		}
	}

	template<class T, class Json>
	void to_json_member(const T& body, Json& json)
	{
		if constexpr (is_optional_v<T>)
		{
			if (body.has_value())
			{
				json = *body;
			}
			else
			{
				json = nullptr;
			}
		}
		else
		{
			json = body;
		}
	}
}

#define GENERATE_FROM_JSON_MEMBER(MemberName) JsonUtility::from_json_member(p.MemberName, json.find(#MemberName), json.end());
#define GENERATE_TO_JSON_MEMBER(MemberName) JsonUtility::to_json_member(MemberName, json[#MemberName]);

#define GENERATE_JSON_BODY(Class, ...) \
static void from_json(const nlohmann::json& json, Class& p) \
{ \
	MACRO_RECURSIVE_FOR_EACH(GENERATE_FROM_JSON_MEMBER, __VA_ARGS__) \
} \
 \
void to_json(nlohmann::json& json) const \
{ \
	MACRO_RECURSIVE_FOR_EACH(GENERATE_TO_JSON_MEMBER, __VA_ARGS__) \
}

class JsonSerializer
{
public:
	template<class T>
	static String Serialize(const T& value)
	{
		return String(nlohmann::json(value).dump());
	}

	template<class T>
	static T Deserialize(const String& value)
	{
		return (T)nlohmann::json::parse(value.AsCodepage(65001));
	}
};