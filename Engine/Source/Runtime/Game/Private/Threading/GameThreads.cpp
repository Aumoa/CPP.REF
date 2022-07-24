// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/GameThreads.h"
#include "Threading/Thread.h"

Thread GameThreads::_gameThread;

void GameThreads::InitializeGameThread()
{
	_gameThread = Thread::GetCurrentThread();
	_gameThread.SetFriendlyName(TEXT("[GameThread]"));
}

bool GameThreads::IsInGameThread()
{
	return _gameThread.GetThreadId() == Thread::GetCurrentThread().GetThreadId();
}