// Copyright 2020 Aumoa.lib. All right reserved.

#include "CoreString.h"

#undef interface
#include <Windows.h>
#include "ArgumentNullException.h"
#include "IndexOutOfRangeException.h"
#include "FormatException.h"

using namespace SC::Runtime::Core;
using namespace std;

wchar_t String::EmptyBuffer[1] = { 0 };
const TRefPtr<String> String::Empty = Object::NewObject<String>(EmptyBuffer);

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

TRefPtr<String> String::ToString() const
{
	return this;
}

bool String::Equals(TRefPtr<Object> right) const
{
	if (TRefPtr<String> ptr; right.Is<String>((String**)&ptr))
	{
		return (*this).operator ==(ptr);
	}
	else
	{
		return Super::Equals(right);
	}
}

size_t String::GetHashCode() const
{
	size_t hash1 = 5381;
	size_t hash2 = hash1;

	size_t	        c;
	const wchar_t*  s = text_buffer;

	while ((c = s[0]) != 0) {
		hash1 = ((hash1 << 5) + hash1) ^ c;
		c = s[1];
		if (c == 0)
			break;
		hash2 = ((hash2 << 5) + hash2) ^ c;
		s += 2;
	}

	return hash1 + (hash2 * 1566083941);
}

String::String(const char* text) : This(text, Strlen(text))
{

}

String::String(const wchar_t* text) : This(text, Strlen(text))
{

}

String::String(const char* text, size_t len) : This()
{
    Assign(text, len);
}

String::String(const wchar_t* text, size_t len) : This()
{
    if (text == EmptyBuffer)
    {
        text_buffer = text;
    }
    else
    {
        Assign(text, len);
    }
}

String::String(const string& text) : This(text.c_str(), text.length())
{

}

String::String(const wstring& text) : This(text.c_str(), text.length())
{

}

String::String(string_view text) : This(text.data(), text.length())
{

}

String::String(wstring_view text) : This(text.data(), text.length())
{

}

auto String::cbegin() const -> ConstIterator
{
    return text_buffer;
}

auto String::cend() const -> ConstIterator
{
    return text_buffer + len;
}

const wchar_t* String::C_Str_get() const
{
	return text_buffer;
}

size_t String::Length_get() const
{
	return len;
}

bool String::operator ==(const TRefPtr<String>& right) const
{
	if (text_buffer == right->text_buffer)
	{
		return true;
	}

	else if (len != right->len)
	{
		return false;
	}

	else
	{
		return memcmp(text_buffer, right->text_buffer, len) == 0;
	}
}

bool String::operator !=(const TRefPtr<String>& right) const
{
	if (text_buffer != right->text_buffer)
	{
		return true;
	}

	else if (len != right->len)
	{
		return true;
	}

	else
	{
		return memcmp(text_buffer, right->text_buffer, len) != 0;
	}
}

wchar_t String::operator [](size_t index) const
{
    if (index > len)
    {
        throw IndexOutOfRangeException();
    }

    return text_buffer[index];
}

TRefPtr<String> String::Format(TRefPtr<String> format)
{
    return format;
}

TRefPtr<String> String::FormatHelper(TRefPtr<String> format, vector<TRefPtr<Object>>& unpackedArgs)
{
    if (!format.IsValid)
    {
        throw ArgumentNullException();
    }
 
    size_t pos = 0;
    size_t len = format->Length;
    wchar_t ch = L'\x0';

    std::wstringstream wss;
 
    while (true)
    {
        size_t p = pos;
        size_t i = pos;

        while (pos < len)
        {
            ch = format[pos];
 
            pos++;
            if (ch == L'}')
            {
                if (pos < len && format[pos] == L'}')
                {
                    pos++;
                }
                else
                {
                    throw FormatException();
                }
            }
 
            if (ch == L'{')
            {
                if (pos < len && format[pos] == L'{')
                {
                    pos++;
                }
                else
                {
                    pos--;
                    break;
                }
            }
 
            wss << ch;
        }
 
        if (pos == len)
        {
            break;
        }

        pos++;
        if (pos == len || (ch = format[pos]) < L'0' || ch > L'9')
        {
            throw FormatException();
        }

        size_t index = 0;
        do
        {
            index = index * 10 + ch - L'0';
            pos++;
            if (pos == len)
            {
                throw FormatException();
            }

            ch = format[pos];
        } while (ch >= L'0' && ch <= L'9' && index < 1000000);

        if (index >= unpackedArgs.size())
        {
            throw FormatException(L"Format index is out of range.");
        }

        while (pos < len && (ch = format[pos]) == L' ')
        {
            pos++;
        }

        bool leftJustify = false;
        int width = 0;
        if (ch == L',')
        {
            pos++;
            while (pos < len && format[pos] == L' ')
            {
                pos++;
            }
 
            if (pos == len)
            {
                throw FormatException();
            }

            ch = format[pos];
            if (ch == L'-')
            {
                leftJustify = true;
                pos++;
                if (pos == len)
                {
                    throw FormatException();
                }
                ch = format[pos];
            }

            if (ch < L'0' || ch > L'9')
            {
                throw FormatException();
            }

            do
            {
                width = width * 10 + ch - L'0';
                pos++;
                if (pos == len)
                {
                    throw FormatException();
                }
                ch = format[pos];
            } while (ch >= L'0' && ch <= L'9' && width < 1000000);
        }
 
        while (pos < len && (ch = format[pos]) == L' ')
        {
            pos++;
        }

        Object* arg = unpackedArgs[index].Get();
        std::wostringstream fmt;
        if (ch == L':')
        {
            pos++;
            p = pos;
            i = pos;
            while (true)
            {
                if (pos == len)
                {
                    throw FormatException();
                }

                ch = format[pos];
                pos++;
                if (ch == L'{')
                {
                    if (pos < len && format[pos] == L'{')
                    {
                        pos++;
                    }
                    else
                    {
                        throw FormatException();
                    }
                }
                else if (ch == L'}')
                {
                    if (pos < len && format[pos] == L'}')
                    {
                        pos++;
                    }
                    else
                    {
                        pos--;
                        break;
                    }
                }
 
                fmt << ch;
            }
        }

        if (ch != L'}')
        {
            throw FormatException();
        }

        pos++;
        wstring s;
        wstring fmtstr = fmt.str();

        if (arg != nullptr)
        {
            if (IStringFormattable* formattable = nullptr; fmtstr.length() != 0 && (formattable = dynamic_cast<IStringFormattable*>(arg)) != nullptr)
            {
                s = formattable->ToString(fmtstr)->C_Str;
            }
            else
            {
                s = arg->ToString()->C_Str;
            }
        }

        ssize_t pad = width - s.length();
        if (!leftJustify && pad > 0)
        {
            size_t repeat = (size_t)pad;
            while (repeat-- != 0)
            {
                wss.put(L' ');
            }
        }

        wss << s;

        if (leftJustify && pad > 0)
        {
            size_t repeat = pad;
            while (repeat-- != 0)
            {
                wss.put(L' ');
            }
        }
    }

    return wss.str();
}

wchar_t* String::MultiByteToWideChar(const char* multibyte, size_t* len)
{
	int length = ::MultiByteToWideChar(CP_ACP, 0, multibyte, (int)*len, nullptr, 0);
    auto* buffer = AllocCharBuffer<wchar_t>(*len + 1);
	::MultiByteToWideChar(CP_ACP, 0, multibyte, (int)*len, buffer, length);
	*len = length;
	return buffer;
}

wchar_t* String::CopyAllocate(const wchar_t* text, size_t len)
{
    auto* buffer = AllocCharBuffer<wchar_t>(len + 1);
	memcpy(buffer, text, sizeof(wchar_t) * len);
	return buffer;
}

void* String::AllocInternal(size_t sizeInBytes)
{
    return new int8[sizeInBytes]{ };
}

void String::FreeInternal(void* internalPointer)
{
    delete[] reinterpret_cast<int8*&>(internalPointer);
}

void String::Assign(const char* text, size_t len)
{
    text_buffer = MultiByteToWideChar(text, &len);
    this->len = len;
    bDynamicBuffer = true;
}

void String::Assign(const wchar_t* text, size_t len)
{
    text_buffer = CopyAllocate(text, len);
    this->len = len;
    bDynamicBuffer = true;
}

size_t String::SizeAsBoundary(size_t len)
{
	size_t rem = len % 4;
	if (rem != 0)
	{
		len += 4 - rem;
	}
	return len;
}