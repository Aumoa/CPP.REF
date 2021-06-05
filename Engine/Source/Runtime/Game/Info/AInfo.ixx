// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AInfo;

import std.core;
import :AActor;

using namespace std;

/// <summary>
/// Represent game information placed to world.
/// </summary>
export class AInfo : public AActor
{
public:
	using Super = AActor;

public:
	AInfo(wstring_view name);
};