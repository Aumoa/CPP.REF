// Copyright 2020 Aumoa.lib. All right reserved.

#include "CoreString.h"

using namespace SC::Runtime::Core;

wchar_t String::EmptyBuffer[1] = { 0 };

extern "C" int __stdcall MultiByteToWideChar(uint32 CodePage, uint32 dwFlags, const char* lpMultiByteStr, int32 cbMultiByte, wchar_t* lpWideCharStr, int32 cchWideChar);
#define CP_ACP 0
extern "C" void memcpy(void* dst, const void* src, size_t len);

String::String() : Super()
	, text_buffer(nullptr)
	, len(0)
	, bDynamicBuffer(false)
{
	text_buffer = EmptyBuffer;
}

String::~String()
{
	if (text_buffer != nullptr && bDynamicBuffer)
	{
		delete[] text_buffer;
		text_buffer = nullptr;
	}
}

String::String(const char* text) : This(text, Strlen(text))
{

}

String::String(const wchar_t* text) : This(text, Strlen(text))
{

}

String::String(const char* text, size_t len) : This()
{
	text_buffer = MultiByteToWideChar(text, &len);
	this->len = len;
	bDynamicBuffer = true;
}

String::String(const wchar_t* text, size_t len) : This()
{
	text_buffer = CopyAllocate(text, len);
	this->len = len;
	bDynamicBuffer = true;
}

const wchar_t* String::C_Str_get() const
{
	return text_buffer;
}

size_t String::Length_get() const
{
	return len;
}

wchar_t* String::MultiByteToWideChar(const char* multibyte, size_t* len)
{
	int length = ::MultiByteToWideChar(CP_ACP, 0, multibyte, (int)*len, nullptr, 0);
	auto* buffer = new wchar_t[(size_t)length + 1];
	::MultiByteToWideChar(CP_ACP, 0, multibyte, (int)*len, buffer, length);
	*len = length;
	buffer[length] = 0;
	return buffer;
}

wchar_t* String::CopyAllocate(const wchar_t* text, size_t len)
{
	auto* buffer = new wchar_t[len + 1];
	memcpy(buffer, text, sizeof(wchar_t) * len);
	buffer[len] = 0;
	return buffer;
}