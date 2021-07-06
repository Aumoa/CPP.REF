// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"
#include "ChessSystemComponent.h"
#include "Queries/ChessQueries.h"
#include <stack>

class CommandComponent : public ChessSystemComponent
{
public:
	using Super = ChessSystemComponent;

private:
	std::stack<ActionRecord> _history;

public:
	CommandComponent();

	virtual void SetupBoard(AChessBoardProxy* proxy) override;
	virtual void DoCommand(const GridIndex& from, const GridIndex& to);
	virtual void Undo();
};