// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Core/String.h"

#undef interface
#include <Windows.h>
#include "ArgumentNullException.h"
#include "IndexOutOfRangeException.h"
#include "FormatException.h"
#include "ArgumentException.h"

using namespace std;

wchar_t String::EmptyBuffer[1] = { 0 };
const TRefPtr<String> String::Empty = Object::NewObject<String>(EmptyBuffer);

#define ThrowIfNull(exp) if ((exp) == nullptr) { throw ArgumentNullException(L"(" L ## #exp L") == nullptr"); }
#define ThrowArg(exp) if (exp) { throw ArgumentException(L ## #exp); }
#define ThrowIfInvalid(exp) if (!exp.IsValid) { throw ArgumentNullException(L"!" L ## #exp); }

namespace
{
    inline bool IsLatin1(wchar_t ch)
    {
        return ch <= '\x00ff';
    }

    inline bool IsWhiteSpaceLatin1(wchar_t ch)
    {
        return (ch == ' ') || (ch >= '\x0009' && ch <= '\x000d') || ch == '\x00a0' || ch == '\x0085';
    }

    inline bool IsWhiteSpace(wchar_t ch)
    {
        if (IsLatin1(ch))
        {
            return IsWhiteSpaceLatin1(ch);
        }
        return false;
    }
}

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
    if (!hash_cache.has_value())
    {
        size_t hash1 = 5381;
        size_t hash2 = hash1;

        size_t	        c;
        const wchar_t* s = text_buffer;

        while ((c = s[0]) != 0) {
            hash1 = ((hash1 << 5) + hash1) ^ c;
            c = s[1];
            if (c == 0)
                break;
            hash2 = ((hash2 << 5) + hash2) ^ c;
            s += 2;
        }

        hash_cache = hash1 + (hash2 * 1566083941);
    }

    return hash_cache.value();
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

TRefPtr<String> String::Substring(size_t startIndex, optional<size_t> length) const
{
    if (!length.has_value())
    {
        length = len - startIndex;
    }

    return NewObject<String>(text_buffer + startIndex, length.value());
}

optional<size_t> String::IndexOf(TRefPtr<String> value, size_t startIndex, bool bIgnoreCase) const
{
    ThrowIfInvalid(value);
    ThrowArg(value->Length == 0);

    for (size_t i = startIndex; i < len; ++i)
    {
        optional<size_t> find = IndexOf(value[0], i, bIgnoreCase);
        // Could'n find any chars.
        if (!find.has_value())
        {
            return nullopt;
        }

        // Find chars but lack of length.
        size_t rem = len - i;
        if (rem < value->Length)
        {
            return nullopt;
        }
        rem = value->Length;

        const size_t seekpos = find.value();
        if (bIgnoreCase)
        {
            bool bSucceeded = true;
            for (size_t j = 0; j < rem; ++j)
            {
                if (tolower(text_buffer[seekpos + j]) != tolower(value[j]))
                {
                    bSucceeded = false;
                    break;
                }
            }

            if (bSucceeded)
            {
                return seekpos;
            }
        }
        else
        {
            if (memcmp(text_buffer + seekpos, value->C_Str, sizeof(wchar_t) * rem) == 0)
            {
                return seekpos;
            }
        }

        i = seekpos + 1;
    }

    return nullopt;
}

optional<size_t> String::IndexOf(wchar_t value, size_t startIndex, bool bIgnoreCase) const
{
    ThrowArg(startIndex >= len);

    for (size_t i = startIndex; i < len; ++i)
    {
        bool bCompare = false;

        if (bIgnoreCase)
        {
            bCompare = tolower(text_buffer[i]) == tolower(value);
        }
        else
        {
            bCompare = text_buffer[i] == value;
        }

        if (bCompare)
        {
            return i;
        }
    }

    return nullopt;
}

optional<size_t> String::IndexOfAny(const wchar_t* value_sequence, size_t length, size_t startIndex, bool bIgnoreCase) const
{
    ThrowArg(startIndex >= len);
    ThrowIfNull(value_sequence);
    ThrowArg(length == 0);

    auto Compare = [&](size_t index, wchar_t value)
    {
        if (bIgnoreCase)
        {
            return tolower(text_buffer[index]) == tolower(value);
        }
        else
        {
            return text_buffer[index] == value;
        }
    };

    for (size_t i = startIndex; i < len; ++i)
    {
        for (size_t j = 0; j < length; ++j)
        {
            if (Compare(i, value_sequence[j]))
            {
                return i;
            }
        }
    }

    return nullopt;
}

string String::AsMultiByte() const
{
    string buf;

    int length = ::WideCharToMultiByte(CP_ACP, 0, text_buffer, (int)len, nullptr, 0, nullptr, FALSE);
    buf.resize(length, ' ');
    ::WideCharToMultiByte(CP_ACP, 0, text_buffer, (int)len, buf.data(), length, nullptr, FALSE);

    return buf;
}

const wchar_t* String::C_Str_get() const
{
	return text_buffer;
}

size_t String::Length_get() const
{
	return len;
}

bool String::operator < (const TRefPtr<String>& right) const
{
    if (!right.IsValid)
    {
        return false;
    }

    int compare = lstrcmpW(C_Str, right->C_Str);
    return compare < 0;
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
    std::vector<int> r;
    std::span<int> g = r;
    return format;
}

TRefPtr<String> String::Join(TRefPtr<String> separator, const span<TRefPtr<String>>& values)
{
    if (values.empty())
    {
        return Empty;
    }

    if (values.size() <= 1)
    {
        return values[0];
    }

    size_t total_length = 0;
    for (size_t i = 0; i < values.size(); ++i)
    {
        total_length += values[i]->Length;
    }

    size_t sep_length = separator->Length;
    total_length += sep_length * (values.size() - 1);

    wchar_t* buffer = new wchar_t[SizeAsBoundary(total_length + 1)];
    size_t   seekpos = 0;

    struct __Finally
    {
        wchar_t* buf;

        __Finally(wchar_t* buf) : buf(buf)
        {

        }

        ~__Finally()
        {
            if (buf != nullptr)
            {
                delete[] buf;
                buf = nullptr;
            }
        }

        void Detach()
        {
            buf = nullptr;
        }
    }
    finally(buffer);

    for (size_t i = 0; i < values.size(); ++i)
    {
        size_t length = values[i]->Length;
        memcpy(buffer + seekpos, values[i]->C_Str, sizeof(wchar_t) * length);
        seekpos += length;

        if (seekpos < total_length)
        {
            memcpy(buffer + seekpos, separator->C_Str, sizeof(wchar_t*) * sep_length);
            seekpos += sep_length;
        }
    }

    buffer[total_length] = 0;

    auto result = NewObject<String>();
    result->text_buffer = buffer;
    result->len = total_length;
    result->bDynamicBuffer = true;
    finally.Detach();

    return move(result);
}

TRefPtr<String> String::Concat(const span<TRefPtr<String>>& values)
{
    if (values.empty())
    {
        return Empty;
    }

    if (values.size() <= 1)
    {
        return values[0];
    }

    size_t total_length = 0;
    for (size_t i = 0; i < values.size(); ++i)
    {
        total_length += values[i]->Length;
    }

    wchar_t* buffer = new wchar_t[SizeAsBoundary(total_length + 1)];
    size_t   seekpos = 0;

    struct __Finally
    {
        wchar_t* buf;

        __Finally(wchar_t* buf) : buf(buf)
        {

        }

        ~__Finally()
        {
            if (buf != nullptr)
            {
                delete[] buf;
                buf = nullptr;
            }
        }

        void Detach()
        {
            buf = nullptr;
        }
    }
    finally(buffer);

    for (size_t i = 0; i < values.size(); ++i)
    {
        size_t length = values[i]->Length;
        memcpy(buffer + seekpos, values[i]->C_Str, sizeof(wchar_t) * length);
        seekpos += length;
    }

    buffer[total_length] = 0;

    auto result = NewObject<String>();
    result->text_buffer = buffer;
    result->len = total_length;
    result->bDynamicBuffer = true;
    finally.Detach();

    return move(result);
}

bool String::IsNullOrEmpty(const TRefPtr<String>& value)
{
    return !value.IsValid || value->Length == 0;
}

bool String::IsNullOrWhiteSpace(const TRefPtr<String>& value)
{
    if (!value.IsValid)
    {
        return true;
    }

    for (size_t i = 0; i < value->Length; i++)
    {
        if (!IsWhiteSpace(value[i]))
        {
            return false;
        }
    }

    return true;
}

TRefPtr<String> String::FormatHelper(TRefPtr<String> format, const span<TRefPtr<Object>>& unpackedArgs)
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