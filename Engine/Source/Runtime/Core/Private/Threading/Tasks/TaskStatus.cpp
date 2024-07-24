// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:TaskStatus;

export enum class ETaskStatus
{
	Created,
	Running,
	RanToCompletion,
	Faulted,
	Canceled,
};