// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Level/Level.h"

class AChessBoard;
class AGridIndicator;

class GameLevel : public SLevel
{
public:
	using Super = SLevel;

private:
	AChessBoard* _psBoard = nullptr;

public:
	GameLevel();

	virtual bool LoadLevel(World* world) override;

	inline AChessBoard* GetPersistentChessBoard() const { return _psBoard; }
};