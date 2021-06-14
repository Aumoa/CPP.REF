// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.Chess:AChessPlayerController;

import SC.Runtime.Game;

export class AChessPlayerController : public APlayerController
{
public:
	using Super = APlayerController;

public:
	AChessPlayerController();

	virtual void BeginPlay() override;
};