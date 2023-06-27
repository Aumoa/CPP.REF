// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:Guid;

export import Core.Std;
export import :IntegralTypes;

export class String;

export class CORE_API Guid
{
	uint32 Data1;
	uint16 Data2;
	uint16 Data3;
	std::array<uint8, 8> Data4;

public:
	constexpr Guid()
		: Data1(0)
		, Data2(0)
		, Data3(0)
		, Data4{}
	{
	}

	constexpr Guid(uint32 Data1, uint16 Data2, uint16 Data3, uint8 Data40, uint8 Data41, uint8 Data42, uint8 Data43, uint8 Data44, uint8 Data45, uint8 Data46, uint8 Data47)
		: Data1(Data1)
		, Data2(Data2)
		, Data3(Data3)
		, Data4{ Data40, Data41, Data42, Data43, Data44, Data45, Data46, Data47 }
	{
	}

	String ToString() const;

	static bool TryParse(String FormattedString, Guid& OutResult);
	static Guid NewGuid();
};