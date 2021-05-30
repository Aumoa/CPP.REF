// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.Chess:ChessGameInstance;

import SC.Runtime.Core;
import SC.Runtime.Game;

export class ChessGameInstance : public GameInstance
{
public:
	using Super = GameInstance;

public:
	ChessGameInstance() : Super(L"Chess")
	{
	}
};