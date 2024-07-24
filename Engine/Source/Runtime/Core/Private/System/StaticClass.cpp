// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:StaticClass;

export class StaticClass
{
	StaticClass() = delete;
	StaticClass(const StaticClass&) = delete;
	StaticClass(StaticClass&&) = delete;

	StaticClass& operator =(const StaticClass&) = delete;
	StaticClass& operator =(StaticClass&&) = delete;
};