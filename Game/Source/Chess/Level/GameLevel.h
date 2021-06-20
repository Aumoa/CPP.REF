// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Level/Level.h"

class AChessBoard;

class GameLevel : public Level
{
public:
	using Super = Level;

private:
	AChessBoard* _psBoard = nullptr;

public:
	GameLevel();

	virtual bool LoadLevel(World* world) override;

	inline AChessBoard* GetPersistentChessBoard() const { return _psBoard; }
};