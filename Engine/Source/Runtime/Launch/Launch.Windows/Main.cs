// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.CoreAObject;

var path = Environment.GetEnvironmentVariable("PATH");

var inst = new AE.CoreAObject.Object();
var locks = inst.locks;

path = null;
