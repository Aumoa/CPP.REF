// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core:FileMode;

export enum class EFileMode
{
	CreateNew = 1,
	Create = 2,
	Open = 3,
	OpenOrCreate = 4,
	Truncate = 5
};