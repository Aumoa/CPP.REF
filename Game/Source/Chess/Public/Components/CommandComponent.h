// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessAIStructures.h"
#include "ChessSystemComponent.h"
#include "Queries/ChessQueries.h"
#include <stack>

class CHESS_API CommandComponent : public ChessSystemComponent
{
	GENERATED_BODY(CommandComponent)

private:
	std::stack<ActionRecord> _history;

public:
	CommandComponent();

	virtual void SetupBoard(AChessBoardProxy* proxy) override;
	virtual void DoCommand(const GridIndex& from, const GridIndex& to);
	virtual void Undo();

public:
	using CommandExecutedEvent = MulticastEvent<CommandComponent, void()>;
	CommandExecutedEvent CommandExecuted;
};