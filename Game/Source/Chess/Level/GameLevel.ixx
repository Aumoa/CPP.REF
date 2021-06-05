// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Game.Chess:GameLevel;

import SC.Runtime.Game;

export class AChessBoard;

export class GameLevel : public Level
{
public:
	using Super = Level;

private:
	AChessBoard* _psBoard = nullptr;

public:
	GameLevel();

	virtual void LoadLevel(World* world) override;

	inline AChessBoard* GetPersistentChessBoard() const { return _psBoard; }
};