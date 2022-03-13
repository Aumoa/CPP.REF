// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include "Misc/String.h"

FileReference::FileReference(const std::filesystem::path& filepath)
	: FileSystemReference(filepath)
{
}

std::filesystem::path FileReference::GetFilename() const
{
	return GetPath().filename();
}

std::filesystem::path FileReference::GetName() const
{
	return GetPath().stem();
}

std::filesystem::path FileReference::GetExtension() const
{
	return GetPath().extension();
}

std::wfstream FileReference::OpenStream(std::ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive) const
{
	if (bCreateIfNotExists && !IsExists())
	{
		if (bCreateDirectoryRecursive)
		{
			GetParent().CreateIfNotExists(bCreateDirectoryRecursive);
		}

		mode = std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
	}

	return std::wfstream(GetPath(), mode);
}

template<class TChar, class... TChars>
inline bool BOMcheck(const char* Orign, TChar&& Char, TChars&&... Chars)
{
	if ((unsigned char)Orign[0] == (unsigned char)Char)
	{
		if constexpr (sizeof...(TChars) != 0)
		{
			return BOMcheck(Orign + 1, std::forward<TChars>(Chars)...);
		}
		
		return true;
	}

	return false;
}

std::wstring FileReference::ReadAllText()
{
	std::ifstream File(GetPath());
	if (File.is_open())
	{
		std::string Buf;
		Buf.resize(File.seekg(0, std::ios::end).tellg());
		File.seekg(0, std::ios::beg);

		if (File.read(Buf.data(), Buf.size()).bad())
		{
			File.close();
			return L"";
		}

		auto Rit = Buf.rbegin();
		for (; Rit != Buf.rend(); ++Rit)
		{
			if (*Rit != 0)
			{
				break;
			}
		}

		Buf.erase(Rit.base(), Buf.end());
		File.close();

		std::wstring Encoded;

		auto EncodeIsUnicode = [&Buf, &Encoded](bool bBigEndian)
		{
			constexpr bool bPlatformBigEndian = std::endian::native == std::endian::big;

			size_t TotalLengthUnicode = Buf.length() / 2 - 1;
			Encoded.resize(TotalLengthUnicode + 1);

			if (bPlatformBigEndian == bBigEndian)
			{
				memcpy(Encoded.data(), Buf.data() + 2, sizeof(wchar_t) * TotalLengthUnicode);
			}
			else
			{
				for (size_t i = 0; i < TotalLengthUnicode; ++i)
				{
					size_t CharIdx = (i + 1) * 2;
					char& BufPtr = Buf[CharIdx];

					// Swap endian.
					std::swap(BufPtr, Buf[CharIdx + 1]);
					Encoded[i] = reinterpret_cast<wchar_t&>(Buf[CharIdx]);
				}
			}
		};

		if (BOMcheck(Buf.data(), 0xEF, 0xBB, 0xBF))
		{
			// UTF-8
			Encoded = String::AsUnicode(Buf.substr(3), 65001);
		}
		else if (BOMcheck(Buf.data(), 0xFE, 0xFF))
		{
			// UTF-16 BE
			EncodeIsUnicode(true);
		}
		else if (BOMcheck(Buf.data(), 0xFF, 0xFE))
		{
			// UTF-16 LE
			EncodeIsUnicode(false);
		}
		else if (BOMcheck(Buf.data(), 0x2B, 0x2F, 0x76))
		{
			// UTF-7
			Encoded = String::AsUnicode(Buf.substr(3), 65000);
		}
		else
		{
			// Without BOM.
			Encoded = String::AsUnicode(Buf);
		}

		return Encoded;
	}
	return L"";
}

bool FileReference::WriteAllText(std::wstring_view Text, uint32 Encoding)
{
	std::string EncodedText = String::AsMultibyte(Text, Encoding);
	std::ofstream File(GetPath(), std::ios::trunc);
	if (File.is_open())
	{
		File << EncodedText;
		File.close();
		return true;
	}
	return false;
}