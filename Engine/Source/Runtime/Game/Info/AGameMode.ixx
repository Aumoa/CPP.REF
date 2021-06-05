// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AGameMode;

import std.core;
import :AInfo;
import :SubclassOf;
import :Level;

using namespace std;

/// <summary>
/// Represents game mode placed to world.
/// </summary>
export class AGameMode : public AInfo
{
public:
	using Super = AInfo;

public:
	AGameMode(wstring_view name);
};