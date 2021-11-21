// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Misc/WordProcessor.h"

WordProcessor::WordProcessor()
{
	IsSeparator = [](char Ch) -> bool
	{
		return (Ch >= 0x21 && Ch <= 0x2F) ||
			   (Ch >= 0x3A && Ch <= 0x40) ||
			   (Ch >= 0x5B && Ch <= 0x60) ||
			   (Ch >= 0x7B && Ch <= 0x7E);
	};

	IsSpace = [](char Ch) -> bool
	{
		return (Ch >= 0x09 && Ch <= 0x0D) || (Ch == 0x20);
	};

	IsNumber = [](char Ch) -> bool
	{
		return (Ch >= 0x30 && Ch <= 0x39);
	};
}

bool WordProcessor::Process(std::span<const uint8> Input, std::function<bool(std::string_view, size_t, size_t)> Processor)
{
	const uint8* Seekp = Input.data();
	const uint8* Endp = Seekp + Input.size_bytes();

	bool bString = false;
	bool bSpecial = false;
	std::vector<char> CharBuf;
	CharBuf.reserve(256);

	size_t Pos = 0;
	size_t Line = 0;

	auto Flush = [&]()
	{
		if (CharBuf.size() == 0)
		{
			return true;
		}

		std::string_view Sv(CharBuf.data(), CharBuf.size());
		if (!Processor(Sv, Line, Pos))
		{
			return false;
		}

		CharBuf.resize(0);
		return true;
	};

#define FLUSH_RET() if (!Flush()) { return false; }

	char Seekl = 0;
	for (; Seekp != Endp; ++Seekp)
	{
		char Seekc = *Seekp;

		if (bString)
		{
			if (Seekc == '\\')
			{
				bSpecial = true;
			}
			else
			{
				CharBuf.emplace_back(Seekc);

				if (Seekc == '\"' && !bSpecial)
				{
					bString = false;

					if (!Flush())
					{
						return false;
					}
				}

				bSpecial = false;
			}
		}
		else
		{
			if (Seekc == '\"' && bProcessString)
			{
				if (!Flush())
				{
					return false;
				}

				CharBuf.emplace_back('\"');
				bString = true;
				bSpecial = false;
			}
			else
			{
				if (IsSpace(Seekc))
				{
					FLUSH_RET();
					continue;
				}
#define FLUSH2_RET() FLUSH_RET(); CharBuf.emplace_back(Seekc); FLUSH_RET();
				else if (Seekc == '.')
				{
					if (!IsNumber(Seekl))
					{
						FLUSH2_RET();
						continue;
					}
				}
				else if (Seekc == '-')
				{
					if (Seekp + 1 == Endp || !IsNumber(*(Seekp + 1)))
					{
						FLUSH2_RET();
						continue;
					}
				}
				else if (IsSeparator(Seekc))
				{
					FLUSH2_RET();
					continue;
				}
#undef FLUSH2_RET

				CharBuf.emplace_back(Seekc);
			}

			Pos += 1;
			if (Seekc == '\n')
			{
				Line += 1;
				Pos = 0;
			}
		}

		Seekl = Seekc;
	}

#undef FLUSH_RET

	return true;
}