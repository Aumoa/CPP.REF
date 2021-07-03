// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "CommandComponent.h"
#include "LogChess.h"
#include "Diagnostics/LogVerbosity.h"

using enum ELogVerbosity;

CommandComponent::CommandComponent() : Super()
{
}

void CommandComponent::SetupBoard(AChessBoardProxy* proxy)
{
	Super::SetupBoard(proxy);
}

void CommandComponent::DoCommand(const GridIndex& from, const GridIndex& to)
{
	LogSystem::Log(LogChess, Verbose, L"DoCommand: {} -> {}", from.ToString(), to.ToString());
}