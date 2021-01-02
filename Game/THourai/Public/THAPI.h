// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __TH_API__
#define TH_API __declspec(dllexport)
#define GEngine (*Engine::GetInstance())
#define GAssetMgr (*GEngine.GetAssetManager())

#include "Common/THLogs.h"
#include "Engine.h"
#include "Assets/AssetManager.h"
#else
#define TH_API __declspec(dllimport)
#endif