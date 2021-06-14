// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.Chess:AChessGameMode;

import :AChessPlayerController;

export class AChessGameMode : public AGameMode
{
public:
	using Super = AGameMode;

public:
	AChessGameMode() : Super()
	{
		PlayerControllerClass = SubclassOf<AChessPlayerController>::StaticClass();
	}
};