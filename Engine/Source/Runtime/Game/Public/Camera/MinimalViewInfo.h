// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Game
{
    struct MinimalViewInfo
    {
        uint8 bInit : 1 = false;
        Vector3 Location;
        Quaternion Rotation;
        float AspectRatio = 1.0f;
        Degrees FOVAngle;
        float NearPlane;
        float FarPlane;
    };
}