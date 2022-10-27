// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NETWORK_API HttpUtility
{
public:
	static String UrlEncode(const String& url) noexcept;
	static String UrlDecode(const String& encodedUrl);
	static inline bool IsUrlSafeChar(char_t ch) noexcept;

private:
	static inline void EncodeChar(unsigned char ch, char* buf) noexcept;
	static inline char DecodeChar(char_t ch1, char_t ch2) noexcept;
};