// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include "Misc/String.h"
#include <bit>
#include <fstream>

using namespace libty;

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

String FileReference::ReadAllText() const
{
	std::ifstream File(this->_Get_path());
	if (File.is_open())
	{
		std::string Buf;
		Buf.resize(File.seekg(0, std::ios::end).tellg());
		File.seekg(0, std::ios::beg);

		if (File.read(Buf.data(), Buf.size()).bad())
		{
			File.close();
			return TEXT("");
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
			Encoded = String::FromCodepage(Buf.substr(3), 65001);
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
			Encoded = String::FromCodepage(Buf.substr(3), 65000);
		}
		else
		{
			// Without BOM.
			Encoded = String(Buf);
		}

		return String(Encoded);
	}
	return TEXT("");
}

bool FileReference::WriteAllText(String Text, uint32 Encoding) const
{
	std::string EncodedText = Text.AsCodepage(Encoding);
	std::ofstream File(this->_Get_path(), std::ios::trunc);
	if (File.is_open())
	{
		File << EncodedText;
		File.close();
		return true;
	}
	return false;
}