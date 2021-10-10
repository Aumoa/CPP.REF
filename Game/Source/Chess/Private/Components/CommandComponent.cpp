// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/CommandComponent.h"
#include "LogChess.h"
#include "Diagnostics/LogVerbosity.h"
#include "Pawns/ChessBoardProxy.h"

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
	ActionRecord simulateRecord = GetProxy()->SimulateMove(from, to);
	if (simulateRecord)
	{
		LogSystem::Log(LogChess, Verbose, L"DoCommand: {} -> {}", from.ToString(), to.ToString());
		_history.emplace(simulateRecord);
		CommandExecuted.Invoke();
	}
	else
	{
		LogSystem::Log(LogChess, Verbose, L"CommandFailed: {} -> {}", from.ToString(), to.ToString());
	}
}

void CommandComponent::Undo()
{
	if (_history.empty())
	{
		return;
	}

	const ActionRecord& record = _history.top();
	record.Undo();
	_history.pop();
}