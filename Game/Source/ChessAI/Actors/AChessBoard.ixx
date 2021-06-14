// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.ChessAI:AChessBoard;

import std.core;
import SC.Runtime.Game;

export class AChessBoardProxy;

using namespace std;
using namespace std::chrono;

export class AChessBoard : public AActor
{
public:
	using Super = AActor;

private:
	Degrees _rotation;

public:
	AChessBoard();

	virtual void TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction) override;

	AChessBoardProxy* CreateProxy();
};